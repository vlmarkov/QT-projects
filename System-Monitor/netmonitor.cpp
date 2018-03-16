#include "netmonitor.h"

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/if_link.h>

const int Kib = 1024;
const int Mib = Kib * 1024;
const int Gib = Mib * 1024;

NetUsage::NetUsage() { }

NetUsage:: NetUsage(const QString _name,
                    const QString _addr,
                    const long    _txBytes,
                    const long    _rxBytes,
                    const double  _txBandwidth,
                    const double  _rxBandwidth): ifName(_name),
                                                 ipAddr(_addr),
                                                 txBytes(_txBytes),
                                                 rxBytes(_rxBytes),
                                                 txBandwidth(_txBandwidth),
                                                 rxBandwidth(_rxBandwidth)
{

}

NetMonitor::NetMonitor(QObject* parent) : QObject(parent)
{
    this->getNetInfo();
}

NetMonitor::~NetMonitor() { }

void NetMonitor::getNetInfo()
{
    char host[NI_MAXHOST] = { 0 };

    struct ifaddrs *ifaddr = nullptr;
    struct ifaddrs *ifa = nullptr;

    int n = 0;

    if (getifaddrs(&ifaddr) == -1)
    {
        throw QString("Can't get ifaddrs info");
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        auto family = ifa->ifa_addr->sa_family;
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
        auto family = ifa->ifa_addr->sa_family;
        if (family == AF_INET/* || family == AF_INET6 */)
        {
            if (getnameinfo(ifa->ifa_addr, (family == AF_INET) ? sizeof(struct sockaddr_in) :
                sizeof(struct sockaddr_in6), host,
                NI_MAXHOST, NULL,
                0, NI_NUMERICHOST) != 0)
            {
                throw QString("Can't get getnameinfo()");
            }

            this->NetUsage_[n++].ipAddr = QString(host);
        }
    }

    freeifaddrs(ifaddr);
}

Q_INVOKABLE void NetMonitor::getNetUsage()
{
    int i = 0;

    struct ifaddrs *ifaddr = nullptr;
    struct ifaddrs *ifa    = nullptr;

    if (getifaddrs(&ifaddr) == -1)
    {
        throw QString("Can't get ifaddrs info");
    }

    for (ifa = ifaddr, i = 0; ifa != NULL; ifa = ifa->ifa_next)
    {
        int family = ifa->ifa_addr->sa_family;
        if (family == AF_PACKET && ifa->ifa_data != NULL)
        {
            auto* stats = (struct rtnl_link_stats *)ifa->ifa_data;

            this->NetUsage_[i].txBandwidth = (stats->tx_bytes - this->NetUsage_[i].txBytes) / (double)Kib;
            this->NetUsage_[i].txBytes     = stats->tx_bytes;
            this->NetUsage_[i].rxBandwidth = (stats->rx_bytes - this->NetUsage_[i].rxBytes) / (double)Kib;
            this->NetUsage_[i].rxBytes     = stats->rx_bytes;
            i++;
        }
    }

    freeifaddrs(ifaddr);
}

Q_INVOKABLE int NetMonitor::getDevices()
{
    return NetUsage_.size();
}

Q_INVOKABLE QString NetMonitor::getDeviceName(const int i)
{
    return this->NetUsage_[i].ifName;
}

Q_INVOKABLE double NetMonitor::getDeviceTxBandwidth(const int i)
{
    return this->NetUsage_[i].txBandwidth;
}

Q_INVOKABLE double NetMonitor::getDeviceRxBandwidth(const int i)
{
    return this->NetUsage_[i].rxBandwidth;
}
