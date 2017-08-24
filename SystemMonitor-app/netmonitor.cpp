#include "netmonitor.h"
#include "qcustomdrawer.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/if_link.h>

#include <algorithm>
#include <vector>
#include <iostream>

NetMonitor::NetMonitor(Ui::MainWindow* ui)
{
    this->userInterface_ = ui;

    this->drawerRx_ = new QCustomDrawer(this->userInterface_->NetUsageGraphRx);
    this->drawerTx_ = new QCustomDrawer(this->userInterface_->NetUsageGraphTx);
}

NetMonitor::~NetMonitor()
{
    delete this->drawerRx_;
    delete this->drawerTx_;
}

void NetMonitor::hwInfoGet()
{
    struct ifaddrs *ifaddr;
    struct ifaddrs *ifa;

    int family, n;

    char host[NI_MAXHOST];

    if (getifaddrs(&ifaddr) == -1)
        throw "[NetMonitor] Can't get ifaddrs info";

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {

        family = ifa->ifa_addr->sa_family;

        if (family == AF_PACKET && ifa->ifa_data != NULL) {
            struct rtnl_link_stats *stats = (struct rtnl_link_stats *)ifa->ifa_data;

            this->NetUsage_.push_back(NetUsage(QString(ifa->ifa_name),
                QString(""), stats->tx_bytes, stats->rx_bytes, 0.0, 0.0));
        }
    }

    for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next) {
        family = ifa->ifa_addr->sa_family;
        if (family == AF_INET/* || family == AF_INET6*/) {
            if (getnameinfo(ifa->ifa_addr, (family == AF_INET) ? sizeof(struct sockaddr_in) :
                sizeof(struct sockaddr_in6), host,
                NI_MAXHOST, NULL,
                0, NI_NUMERICHOST) != 0)
            {
                throw "[NetMonitor] getnameinfo() failed";
            }

            this->NetUsage_[n++].ipAddr_ = QString(host);
        }
    }

    freeifaddrs(ifaddr);
}

void NetMonitor::hwInfoShow()
{
    // TODO
    return;
}

void NetMonitor::hwUsageGather(bool activate)
{
    if (!activate)
        return;

    this->createGraph();
}

void NetMonitor::hwUsageShow()
{
    static QTime time(QTime::currentTime());
    static double lastTimePoint = 0;

    // Calculate two new data points:
    double timePoint = time.elapsed()/1000.0;

    if (timePoint - lastTimePoint > 1.0) {

        this->netStatGet();

        auto i = 0;

        for_each(this->NetUsage_.begin(), this->NetUsage_.end(), [&](NetUsage object) {
            this->drawerRx_->setPlotData(timePoint, object.rxBandwidth_, i);
            this->drawerTx_->setPlotData(timePoint, object.txBandwidth_, i);
            i++;
        });

        lastTimePoint = timePoint;

        this->drawerRx_->replotCustomPlot(timePoint);
        this->drawerTx_->replotCustomPlot(timePoint);
    }
}

void NetMonitor::createGraph()
{
    QString titleTextRx("Network Resource Usage (download)");
    QString titleTextTx("Network Resource Usage (upload)");
    QString yLabelText("Bandwidth, kib/sec");
    QString xLabelText("Time, sec");
    std::pair<double, double> yRange(0.0, 10000.0);
    int dataSize = (int)this->NetUsage_.size();

    this->drawerRx_->createCustomPlot(titleTextRx, yLabelText, xLabelText, yRange, dataSize);
    this->drawerRx_->connectSignalSlot();

    this->drawerTx_->createCustomPlot(titleTextTx, yLabelText, xLabelText, yRange, dataSize);
    this->drawerTx_->connectSignalSlot();

    auto i = 0;
    for_each(this->NetUsage_.begin(), this->NetUsage_.end(), [&](NetUsage object)
    {
        QString name;

        name += object.ifName_;
        name += QString(" (");
        name += object.ipAddr_;
        name += QString(")");

        this->drawerRx_->setPlotName(name, i);
        this->drawerRx_->setPlotColor(i);

        this->drawerTx_->setPlotName(name, i);
        this->drawerTx_->setPlotColor(i);
        i++;
    });
}

void NetMonitor::netStatGet()
{
    auto i = 0;
    struct ifaddrs *ifaddr, *ifa;

    if (getifaddrs(&ifaddr) == -1)
        throw "[NetMonitor] Can't get ifaddrs info";

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {

        int family = ifa->ifa_addr->sa_family;

        if (family == AF_PACKET && ifa->ifa_data != NULL) {

            struct rtnl_link_stats *stats = (struct rtnl_link_stats *)ifa->ifa_data;

            this->NetUsage_[i].txBandwidth_ = (stats->tx_bytes - this->NetUsage_[i].txBytes_) / (double)Kib;
            this->NetUsage_[i].txBytes_     = stats->tx_bytes;

            this->NetUsage_[i].rxBandwidth_ = (stats->rx_bytes - this->NetUsage_[i].rxBytes_) / (double)Kib;
            this->NetUsage_[i].rxBytes_     = stats->rx_bytes;
            i++;
        }
    }

    freeifaddrs(ifaddr);
}
