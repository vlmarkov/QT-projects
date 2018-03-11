#include "gpumonitor.h"

#include <iostream>
#include <fstream>

GpuMonitor::GpuMonitor(Ui::MainWindow* ui)
{
    this->userInterface_ = ui;
}

GpuMonitor::~GpuMonitor()
{
    // TODO
}

void GpuMonitor::hwInfoGet()
{
    if (system("lspci -v | grep VGA -A 2 > gpuRaw.data")  != 0) {
        throw "Can't get gpu info";
    }

    std::ifstream gpuRawFile("gpuRaw.data", std::ios::in);
    if (!gpuRawFile.is_open()) {
        throw "Can't read gpu info";
    }

    std::string line;
    while (getline(gpuRawFile, line)) {
        std::size_t model = line.find("VGA");
        if (model != std::string::npos) {
            line.erase(line.begin(), line.begin() + model);
            model = line.find(":");
            line.erase(line.begin(), line.begin() + model + 2);
            this->modelName_ = QString::fromUtf8(line.c_str());
            continue;
        }

        std::size_t memory = line.find("Memory");
        if (memory != std::string::npos) {
            memory = line.find("size=");
            line.erase(line.begin(), line.begin() + memory + 5);
            line.erase(line.end() - 2, line.end());
            this->totalMemory_ = QString::fromUtf8(line.c_str());
            continue;
        }

    }
    gpuRawFile.close();

    system("rm -f gpuRaw.data");
}

void GpuMonitor::hwInfoShow()
{
    this->userInterface_->GpuModel_txt->setText(this->modelName_);
    this->userInterface_->GpuMemory_txt->setText(this->totalMemory_);
}

void GpuMonitor::hwUsageGather(bool activate)
{
    return;
}

void GpuMonitor::hwUsageShow()
{
    return;
}
