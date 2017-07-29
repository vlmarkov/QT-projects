#include "cpumonitor.h"

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>
#include <chrono>
#include <thread>

#define HW_CPU_INFO_FILE "hwCpuInfo.txt"

CpuMonitor::CpuMonitor(Ui::MainWindow* ui)
{
    this->ui_ = ui;
}

CpuMonitor::~CpuMonitor()
{
    this->cpuUsageTread_->join();
    delete this->cpuUsageTread_;
}

void CpuMonitor::hwInfoGet()
{
    try {
        if (system("lscpu > "HW_CPU_INFO_FILE) != 0) {
            throw "Can't get cpu info from os";
        }

        std::ifstream hwCpuInfoFile(HW_CPU_INFO_FILE, std::ios::in);
        if (hwCpuInfoFile.is_open()) {
            std::string line;
            int lnNum = 0;
            while (getline(hwCpuInfoFile, line)) {
                this->hwInfoAsign(line, lnNum++);
            }
            hwCpuInfoFile.close();
            system("rm -f "HW_CPU_INFO_FILE);
        } else {
            throw "Can't read cpu info file";
        }
    } catch (std::string err) {
        std::cerr << err << std::endl;
    }
}

void CpuMonitor::hwInfoAsign(std::string line, int lnNum)
{
    std::size_t found = line.find(":");
    if (found == std::string::npos)
        return;

    for (int i = found + 1; i < line.size(); ++i) {
        if (line[i] != ' ') {
            found = i;
            break;
        }
    }

    line.erase(line.begin(), line.begin() + found);

    switch(lnNum) {
        case 0 : this->arch_ = line; break;
        case 1 : this->bit_  = line; break;
        case 2 : this->byte_ = line; break;
        case 3 : this->cores_  = line; break;
        case 4 : break;
        case 5 : this->threads_ = line; break;
        case 6 :
        case 8 : break;
        case 9 : this->vendor_ = line; break;
        case 10:
        case 11: break;
        case 12: this->model_ = line; break;
        case 13: break;
        case 14: this->cpu_ = line; break;
        case 15:
        case 17: break;
        case 18: this->l1d_ = line; break;
        case 19: this->l1i_ = line; break;
        case 20: this->l2_ = line; break;
        case 21: this->l3_ = line; break;
        case 22: break;
        case 23: this->flags_ = line; break;
    }
}

void CpuMonitor::hwInfoShow()
{
/*
    std::cout << "Arch      : " << this->arch_ << std::endl;
    std::cout << "Bit       : " << this->bit_  << std::endl;
    std::cout << "Byte order: " << this->byte_ << std::endl;
    std::cout << "Cores     : " << this->cores_  << std::endl;
    std::cout << "Threads   : " << this->threads_ << std::endl;
    std::cout << "Vendor    : " << this->vendor_ << std::endl;
    std::cout << "Model name: " << this->model_ << std::endl;
    std::cout << "Cpu freq  : " << this->cpu_ << std::endl;
    std::cout << "L1d       : " << this->l1d_ << std::endl;
    std::cout << "L1i       : " << this->l1i_ << std::endl;
    std::cout << "L2        : " << this->l2_ << std::endl;
    std::cout << "L3        : " << this->l3_ << std::endl;
    std::cout << "Flags     : " << this->flags_ << std::endl;
*/

    this->ui_->Arch_txt->setText(QString::fromUtf8(this->arch_.c_str()));
    this->ui_->Vendor_txt->setText(QString::fromUtf8(this->vendor_.c_str()));
    this->ui_->Model_txt->setText(QString::fromUtf8(this->model_.c_str()));
    this->ui_->Cores_txt->setText(QString::fromUtf8(this->cores_.c_str()));
}

void CpuMonitor::hwUsageGather(bool activate)
{
    int time = 0;
    std::ofstream plotFile;

    plotFile.open ("CpuUsage.dat");
    plotFile.close();

    while (time != 10) {
        plotFile.open ("CpuUsage.dat", std::ios::app);
        this->hwInfoGet();
        plotFile << time << "\t" << this->cpu_ << std::endl;
        plotFile.close();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        time++;
    }
}

void CpuMonitor::hwUsageShowRealTime()
{
    while (1) {
        this->ui_->lcdNumber->display(std::stoi(this->cpu_));
        std::this_thread::sleep_for(std::chrono::seconds(1));

    }
}

void CpuMonitor::hwUsageShow()
{
    //system("./plot_script.sh");
    //system("rm -f CpuUsage.dat");
    //this->hwUsageShowRealTime();
    this->cpuUsageTread_= new std::thread(&CpuMonitor::hwUsageShowRealTime, this);
    //cpuUsage.detach();
}
