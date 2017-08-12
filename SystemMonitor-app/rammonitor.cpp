#include "rammonitor.h"

#include <sys/sysinfo.h>

RamMonitor::RamMonitor(Ui::MainWindow* ui)
{
    this->userInterface_ = ui;
}

RamMonitor::~RamMonitor()
{
    // TODO
}

void RamMonitor::hwInfoGet()
{
    struct sysinfo sysInfo;

    if (sysinfo(&sysInfo) != 0) {
        throw "Can't get sysinfo()";
    }

    this->totalRam_  = sysInfo.totalram  / Kib;
    this->freeRam_   = sysInfo.freeram   / Kib;
    this->sharedRam_ = sysInfo.sharedram / Kib;
    this->bufferRam_ = sysInfo.bufferram / Kib;
    this->totalSwap_ = sysInfo.totalswap / Kib;
    this->freeSwap_  = sysInfo.freeswap  / Kib;
}

void RamMonitor::hwInfoShow()
{
    this->userInterface_->TotalRam_txt->setText(std::to_string(this->totalRam_).c_str());
    this->userInterface_->FreeRam_txt->setText(std::to_string(this->freeRam_).c_str());
    this->userInterface_->SharedRam_txt->setText(std::to_string(this->sharedRam_).c_str());
    this->userInterface_->BufferedRam_txt->setText(std::to_string(this->bufferRam_).c_str());
    this->userInterface_->TotalSwap_txt->setText(std::to_string(this->totalSwap_).c_str());
    this->userInterface_->FreeSwapRam_txt->setText(std::to_string(this->freeSwap_).c_str());
}

void RamMonitor::hwUsageGather(bool activate)
{

}

void RamMonitor::hwUsageShow()
{

}
