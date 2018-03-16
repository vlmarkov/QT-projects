#include "hddmonitor.h"

#include <QString>

#include <sys/statvfs.h>

const int Kib = 1024;
const int Mib = Kib * 1024;
const int Gib = Mib * 1024;

HddMonitor::HddMonitor(QObject* parent) : QObject(parent)
{
    this->hwInfoGet();
}

HddMonitor::~HddMonitor() { }

void HddMonitor::hwInfoGet()
{
    struct statvfs statFs;

    if (statvfs("/", &statFs) == -1)
    {
        throw QString("Can't get statvfs()");
    }

    this->blocksSize_  = statFs.f_bsize;
    this->blocksFree_  = (statFs.f_bavail * statFs.f_bsize) / Mib;
    this->blocksTotal_ = (statFs.f_blocks * statFs.f_bsize) / Mib;
}

Q_INVOKABLE double HddMonitor::getFreeSpace()
{
    return ((double)this->blocksFree_ / (double)this->blocksTotal_ * 100.0);
}

Q_INVOKABLE double HddMonitor::getUseSpace()
{
    return 100.0 - ((double)this->blocksFree_ / (double)this->blocksTotal_ * 100.0);
}

Q_INVOKABLE QString HddMonitor::getFreeSpaceStr()
{
    return QString("Free " + QString::number(this->blocksFree_) + " Mib");
}

Q_INVOKABLE QString HddMonitor::getUseSpaceStr()
{
    return QString("Use " + QString::number(this->blocksTotal_ - this->blocksFree_) + " Mib");
}

Q_INVOKABLE QString HddMonitor::getSpaceStr()
{
    return QString(QString::number(this->blocksTotal_) + " Mib");
}
