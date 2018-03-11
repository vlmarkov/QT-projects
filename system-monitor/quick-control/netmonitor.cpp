#include "netmonitor.h"
#include "abstractsystemmonitor.h"

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/if_link.h>

#include <algorithm>
#include <iostream>

NetMonitor::NetMonitor(QObject* parent) : QObject(parent)
{
    this->getNetInfo();
}

NetMonitor::~NetMonitor() { }

void NetMonitor::getNetInfo()
{
    struct ifaddrs *ifaddr;
    struct ifaddrs *ifa;

    int family, n;

    char host[NI_MAXHOST];

    if (getifaddrs(&ifaddr) == -1)
    {
        throw QString("[NetMonitor] Can't get ifaddrs info");
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        family = ifa->ifa_addr->sa_family;
        if (family == AF_PACKET && ifa->ifa_data != NULL)
        {
            struct rtnl_link_stats *stats = (struct rtnl_link_stats *)ifa->ifa_data;

            this->NetUsage_.push_back(NetUsage(QString(ifa->ifa_name),
                                               QString(""),
                                               stats->tx_bytes,
                                               stats->rx_bytes,
                                               0.0,
                                               0.0));
        }
    }

    for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next)
    {
        family = ifa->ifa_addr->sa_family;
        if (family == AF_INET/* || family == AF_INET6 */)
        {
            if (getnameinfo(ifa->ifa_addr, (family == AF_INET) ? sizeof(struct sockaddr_in) :
                sizeof(struct sockaddr_in6), host,
                NI_MAXHOST, NULL,
                0, NI_NUMERICHOST) != 0)
            {
                throw QString("[NetMonitor] getnameinfo() failed");
            }

            this->NetUsage_[n++].ipAddr_ = QString(host);
        }
    }

    freeifaddrs(ifaddr);
}

Q_INVOKABLE void NetMonitor::getNetUsage()
{
    auto i = 0;
    struct ifaddrs *ifaddr, *ifa;

    if (getifaddrs(&ifaddr) == -1)
    {
        throw "[NetMonitor] Can't get ifaddrs info";
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        int family = ifa->ifa_addr->sa_family;
        if (family == AF_PACKET && ifa->ifa_data != NULL)
        {
            struct rtnl_link_stats *stats = (struct rtnl_link_stats *)ifa->ifa_data;

            this->NetUsage_[i].txBandwidth_ = (stats->tx_bytes - this->NetUsage_[i].txBytes_) / (double)Kib;
            this->NetUsage_[i].txBytes_     = stats->tx_bytes;
            this->NetUsage_[i].rxBandwidth_ = (stats->rx_bytes - this->NetUsage_[i].rxBytes_) / (double)Kib;
            this->NetUsage_[i].rxBytes_     = stats->rx_bytes;
            i++;
        }
    }

    freeifaddrs(ifaddr);

    //std::cout << __FUNCTION__ << std::endl;
}

Q_INVOKABLE int NetMonitor::getDevices()
{
    return NetUsage_.size();
}

Q_INVOKABLE QString NetMonitor::getDeviceName(int device)
{
    return this->NetUsage_[device].ifName_;
}

Q_INVOKABLE double NetMonitor::getDeviceTxBandwidth(int device)
{
    return this->NetUsage_[device].txBandwidth_;
}

Q_INVOKABLE double NetMonitor::getDeviceRxBandwidth(int device)
{
    //std::cout << device << ": " << this->NetUsage_[device].rxBandwidth_ << std::endl;
    return this->NetUsage_[device].rxBandwidth_;
}
