#include "rammonitor.h"

#include <sys/sysinfo.h>

const int Kib = 1024;
const int Mib = Kib * 1024;
const int Gib = Mib * 1024;

RamMonitor::RamMonitor(QObject* parent) : QObject(parent)
{
    this->hwInfoGet();
}

RamMonitor::~RamMonitor() { }

void RamMonitor::hwInfoGet()
{
    struct sysinfo sysInfo;

    if (sysinfo(&sysInfo) != 0)
    {
        throw QString("Can't get ram info");
    }

    this->totalRam_  = (double)(sysInfo.totalram  * (unsigned long long)sysInfo.mem_unit) / (double)Gib;
    this->freeRam_   = (double)(sysInfo.freeram   * (unsigned long long)sysInfo.mem_unit) / (double)Gib;
    this->sharedRam_ = (double)(sysInfo.sharedram * (unsigned long long)sysInfo.mem_unit) / (double)Gib;
    this->bufferRam_ = (double)(sysInfo.bufferram * (unsigned long long)sysInfo.mem_unit) / (double)Gib;
    this->totalSwap_ = (double)(sysInfo.totalswap * (unsigned long long)sysInfo.mem_unit) / (double)Gib;
    this->freeSwap_  = (double)(sysInfo.freeswap  * (unsigned long long)sysInfo.mem_unit) / (double)Gib;
}

Q_INVOKABLE void RamMonitor::updateHwInfo()
{
    return this->hwInfoGet();
}

Q_INVOKABLE double RamMonitor::getTotalRam()
{
    return this->totalRam_;
}

Q_INVOKABLE double RamMonitor::getUsedRam()
{
    return this->totalRam_ - this->freeRam_;
}

Q_INVOKABLE double RamMonitor::getFreeRam()
{
    return this->freeRam_;
}

Q_INVOKABLE double RamMonitor::getSharedRam()
{
    return this->sharedRam_;
}

Q_INVOKABLE double RamMonitor::getBufferedRam()
{
    return this->bufferRam_;
}

Q_INVOKABLE double RamMonitor::getTotalSwap()
{
    return this->totalSwap_;
}

Q_INVOKABLE double RamMonitor::getUsedSwap()
{
    return this->totalSwap_ - this->freeSwap_;
}

Q_INVOKABLE double RamMonitor::getFreeSwap()
{
    return this->freeSwap_;
}
