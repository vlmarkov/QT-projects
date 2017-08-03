#include "cpumonitor.h"
#include "qcustomplot.h"

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
    // TODO
}

void CpuMonitor::hwInfoGet()
{

    try {
        if (system("cat /proc/cpuinfo > "HW_CPU_INFO_FILE) != 0) {
            throw "Can't get cpu info from os";
        }

        std::ifstream hwCpuInfoFile(HW_CPU_INFO_FILE, std::ios::in);
        if (hwCpuInfoFile.is_open()) {
            std::string line;
            int lnNum = 0;
            while (getline(hwCpuInfoFile, line)) {
                this->hwInfoAsign(line, lnNum++);
                if (lnNum == 28) {
                    lnNum = 0;
                }
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

    switch (lnNum)
    {
        //case 0 : std::cout << line << std::endl; break;
        case 1 : this->vendorId_  = line; break;
        case 4 : this->modelName_ = line; break;
        case 7 : cpuFrequency_.push_back(line); break;
        case 12: this->cpuCores_ = line; break;
        case 19: this->flags_ = line; break;
    }

}

void CpuMonitor::hwInfoShow()
{
    //this->ui_->Arch_txt->setText(QString::fromUtf8(this->arch_.c_str()));
    this->ui_->Vendor_txt->setText(QString::fromUtf8(this->vendorId_.c_str()));
    this->ui_->Model_txt->setText(QString::fromUtf8(this->modelName_.c_str()));
    this->ui_->Cores_txt->setText(QString::fromUtf8(this->cpuCores_.c_str()));

    for (auto i = 0; i != cpuFrequency_.size(); ++i) {
        this->ui_->widget->addGraph();
        switch (i)
        {
            case 0: this->ui_->widget->graph(i)->setPen(QPen(QColor(0, 0, 255))); break;
            case 1: this->ui_->widget->graph(i)->setPen(QPen(QColor(0, 255, 0))); break;
            case 2: this->ui_->widget->graph(i)->setPen(QPen(QColor(255, 0, 0))); break;
            case 3: this->ui_->widget->graph(i)->setPen(QPen(QColor(150, 0, 255))); break;
            case 4: this->ui_->widget->graph(i)->setPen(QPen(QColor(0, 150, 255))); break;
            case 5: this->ui_->widget->graph(i)->setPen(QPen(QColor(255, 0, 150))); break;
            case 6: this->ui_->widget->graph(i)->setPen(QPen(QColor(255, 200, 200))); break;
            case 7: this->ui_->widget->graph(i)->setPen(QPen(QColor(10, 250, 200))); break;
            defualt: this->ui_->widget->graph(i)->setPen(QPen(QColor(0, 0, 0))); break;
        }
    }
}

void CpuMonitor::hwUsageGather(bool activate)
{
    return;
}

void CpuMonitor::hwUsageShowRealTime()
{
    static QTime time(QTime::currentTime());
    static long i = 0;

    // Calculate two new data points:
    double key = time.elapsed()/1000.0;
    static double lastPointKey = 0;

    if (key-lastPointKey > 0.002) // Every 2 ms
    {
        this->hwInfoGet();
        for (auto i = 0; i != cpuFrequency_.size(); ++i) {
            this->ui_->widget->graph(i)->addData(key,
                stod(this->cpuFrequency_[i]));
        }
        lastPointKey = key;
        i++;
    }
    this->ui_->widget->xAxis->setRange(key, 8, Qt::AlignRight);
    this->ui_->widget->replot();
    this->cpuFrequency_.clear();

}

void CpuMonitor::hwUsageShow()
{
    this->hwUsageShowRealTime();
}
