#include "osmonitor.h"
#include "abstractsystemmonitor.h"

#include <fstream>
#include <iostream>

OsMonitor::OsMonitor(QObject* parent) : QObject(parent)
{
    this->getHwInfo();
}

OsMonitor::~OsMonitor() { }

void OsMonitor::getHwInfo()
{
    uname(&this->utsname_);

    this->pw_ = getpwuid(geteuid());

    sysinfo(&this->info_);
}

Q_INVOKABLE QString OsMonitor::getOsNameStr()
{
    return QString(this->utsname_.sysname);
}

Q_INVOKABLE QString OsMonitor::getHostNameStr()
{
    return QString(this->utsname_.nodename);
}

Q_INVOKABLE QString OsMonitor::getUserNameStr()
{
    if (this->pw_)
    {
        return QString(this->pw_->pw_name);
    }
    else
    {
        return QString("n/a");
    }
}

Q_INVOKABLE QString OsMonitor::getUpTimeStr()
{
    if (this->info_.uptime > 3600)
    {
        auto min = this->info_.uptime / 60;
        auto sec = this->info_.uptime % 60;
        auto hr  = min / 60;
        min %= 60;
        return QString(QString::number(hr) + " hour, " +
                       QString::number(min) + " min, " +
                       QString::number(sec) + " sec");
    }
    else
    {
        auto min = this->info_.uptime / 60;
        auto sec = this->info_.uptime % 60;
        return QString(QString::number(min) + " min, " +
                       QString::number(sec) + " sec");
    }
}

Q_INVOKABLE QString OsMonitor::getTotalRamStr()
{
    if (this->info_.totalram / Gib > 1)
    {
        return QString::number((double)this->info_.totalram / (double)Gib) + " Gib";
    }
    else if (this->info_.totalram / Mib > 1)
    {
        return QString::number((double)this->info_.totalram / (double)Mib) + " Mib";
    }
    else
    {
        return QString::number((double)this->info_.totalram / (double)Kib) + " Kib";
    }
}

Q_INVOKABLE QString OsMonitor::getCpuNameStr()
{
    const auto cpuNameOffset = 16;
    std::ifstream stream("/proc/cpuinfo");
    std::string str;

    for (auto i = 0; i < cpuNameOffset; i++)
    {
        stream >> str;
    }

    std::getline(stream, str);

    return QString(str.c_str());
}

