#include "traffic_monitor.h"

#include <cstring>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/if_link.h>

TrafficMonitor::TrafficMonitor()
{
    ;
}

TrafficMonitor::TrafficMonitor(const QVector<TrafficUsage>& rhs)
{
    if (rhs.size() != 0)
    {
        this->usage_ = rhs;
    }
}

TrafficMonitor::~TrafficMonitor()
{
    ;
}

QVector<TrafficUsage> TrafficMonitor::getNetworkUsage()
{
    struct ifaddrs* ifaddr = nullptr;

    if (getifaddrs(&ifaddr) == -1)
    {
        throw QString("Can't getifaddrs()");
    }

    QVector<TrafficUsage> v;

    for (auto* ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr->sa_family == AF_PACKET && ifa->ifa_data != nullptr)
        {
            if (std::strcmp(ifa->ifa_name, "lo") == 0)
            {
                continue;
            }

            auto tx = ((struct rtnl_link_stats* )ifa->ifa_data)->tx_bytes;
            auto rx = ((struct rtnl_link_stats* )ifa->ifa_data)->rx_bytes;

            v.push_back(TrafficUsage(ifa->ifa_name, rx, tx));
        }
    }

    freeifaddrs(ifaddr);

    this->syncTraffic_(v);

    return v;
}

void TrafficMonitor::syncTraffic_(QVector<TrafficUsage>& rhs)
{
    if (rhs.size() == this->usage_.size())
    {
        for (auto i = 0; i < rhs.size(); ++i)
        {
            if (std::strcmp(rhs[i].ifName, this->usage_[i].ifName) == 0)
            {
                rhs[i].rxBytes += this->usage_[i].rxBytes;
                rhs[i].txBytes += this->usage_[i].txBytes;
            }
        }
    }
}
