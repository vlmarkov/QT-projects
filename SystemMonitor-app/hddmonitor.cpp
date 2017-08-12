#include "hddmonitor.h"

#include <sys/statvfs.h>

#include <iostream>

HddMonitor::HddMonitor(Ui::MainWindow* ui)
{
    this->userInterface_ = ui;
}

HddMonitor::~HddMonitor()
{
    // TODO
}

void HddMonitor::hwInfoGet()
{
    struct statvfs statFs;

    if (statvfs("/", &statFs) == -1) {
        throw "Can't get statvfs()";
    }

    this->blocksSize_  = statFs.f_bsize;
    this->blocksFree_  = (statFs.f_bavail * statFs.f_bsize) / Mib;
    this->blocksTotal_ = (statFs.f_blocks * statFs.f_bsize) / Mib;
}

void HddMonitor::hwInfoShow()
{
    this->userInterface_->TotalSpace_txt ->setText(std::to_string(this->blocksTotal_).c_str());
    this->userInterface_->FreeSpace_txt->setText(std::to_string(this->blocksFree_).c_str());

    int usage = (((double)this->blocksTotal_) / ((double)this->blocksFree_)) * 10;

    this->userInterface_->UsageSpace_txt->setText(std::to_string(usage).c_str());
}

void HddMonitor::hwUsageGather(bool activate)
{

}

void HddMonitor::hwUsageShow()
{

}

