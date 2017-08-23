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
        throw "[RamMonitor] Can't get ram info";
    }

    this->totalRam_  = QString::number(sysInfo.totalram  / Kib);
    this->freeRam_   = QString::number(sysInfo.freeram   / Kib);
    this->sharedRam_ = QString::number(sysInfo.sharedram / Kib);
    this->bufferRam_ = QString::number(sysInfo.bufferram / Kib);
    this->totalSwap_ = QString::number(sysInfo.totalswap / Kib);
    this->freeSwap_  = QString::number(sysInfo.freeswap  / Kib);
}

void RamMonitor::hwInfoShow()
{
    auto ui = this->userInterface_;

    ui->TotalRam_txt->setText(this->totalRam_);
    ui->FreeRam_txt->setText(this->freeRam_);
    ui->SharedRam_txt->setText(this->sharedRam_);
    ui->BufferedRam_txt->setText(this->bufferRam_);
    ui->TotalSwap_txt->setText(this->totalSwap_);
    ui->FreeSwapRam_txt->setText(this->freeSwap_);
}

void RamMonitor::hwUsageGather(bool activate)
{
    return;
}

void RamMonitor::hwUsageShow()
{
    return;
}
