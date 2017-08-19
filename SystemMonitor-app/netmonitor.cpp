#include "netmonitor.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/if_link.h>

#include <iostream>

NetMonitor::NetMonitor(Ui::MainWindow* ui)
{
    this->userInterface_ = ui;
}

NetMonitor::~NetMonitor()
{
    delete this->titleRx_;
    delete this->subLayoutRx_;
    delete this->titleTx_;
    delete this->subLayoutTx_;
}

void NetMonitor::hwInfoGet()
{
    struct ifaddrs *ifaddr;
    struct ifaddrs *ifa;

    int family, s, n;

    char host[NI_MAXHOST];

    if (getifaddrs(&ifaddr) == -1)
    {
        throw "getifaddrs";
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {

        family = ifa->ifa_addr->sa_family;

        if (family == AF_PACKET && ifa->ifa_data != NULL)
        {
            struct rtnl_link_stats *stats = (struct rtnl_link_stats *)ifa->ifa_data;

            class NetUsage iface_stat;
            iface_stat.ifName_      = QString(ifa->ifa_name);
            iface_stat.txBandwidth_ = 0.0;
            iface_stat.txBandwidth_ = 0.0;
            iface_stat.txBytes_     = stats->tx_bytes;
            iface_stat.rxBytes_     = stats->rx_bytes;

            this->NetUsage_.push_back(iface_stat);
        }
    }

    for (ifa = ifaddr, n = 0; ifa != NULL; ifa = ifa->ifa_next)
    {
        family = ifa->ifa_addr->sa_family;
        if (family == AF_INET/* || family == AF_INET6*/)
        {
            if(getnameinfo(ifa->ifa_addr, (family == AF_INET) ? sizeof(struct sockaddr_in) :
                sizeof(struct sockaddr_in6), host,
                NI_MAXHOST, NULL,
                0, NI_NUMERICHOST) != 0)
            {
                throw "getnameinfo() failed";
            }
            this->NetUsage_[n++].ipAddr_ = QString(host);
        }
    }
    freeifaddrs(ifaddr);
}

void NetMonitor::hwInfoShow()
{
    /*
    for (auto i = this->NetUsage_.begin(); i != this->NetUsage_.end(); ++i)
    {
        std::cout << i->ifName_.toUtf8().constData() << ": Rx " << i->txBandwidth_  / 1024.0 << std::endl;
        std::cout << i->ifName_.toUtf8().constData() << ": Tx " << i->rxBandwidth_  / 1024.0 << std::endl;
        std::cout << std::endl;
    }

    std::cout << this->NetUsage_.size() << std::endl;
    */
}

void NetMonitor::hwUsageGather(bool activate)
{
    if (!activate)
        return;

    this->createGraph();
    this->connectSignalSlot();
}

void NetMonitor::hwUsageShow()
{
    static QTime time(QTime::currentTime());
    static double lastTimePoint = 0;

    // Calculate two new data points:
    double timePoint = time.elapsed()/1000.0;

    if (timePoint - lastTimePoint > 1.0)
    {
        this->netStatGet();
        for (auto i = 0; i < this->NetUsage_.size(); ++i)
        {
            //std::cout << this->NetUsage_[i].ifName_.toUtf8().constData();
            //std::cout << ": Rx " << this->NetUsage_[i].rxBandwidth_ << std::endl;

            this->userInterface_->NetUsageGraphRx->graph(i)->addData(timePoint,
                this->NetUsage_[i].rxBandwidth_);

            this->userInterface_->NetUsageGraphTx->graph(i)->addData(timePoint,
                this->NetUsage_[i].txBandwidth_);
        }
        lastTimePoint = timePoint;
        this->userInterface_->NetUsageGraphRx->xAxis->setRange(timePoint, 100, Qt::AlignRight);
        this->userInterface_->NetUsageGraphRx->replot();

        this->userInterface_->NetUsageGraphTx->xAxis->setRange(timePoint, 100, Qt::AlignRight);
        this->userInterface_->NetUsageGraphTx->replot();
    }
}

void NetMonitor::createGraph()
{
    auto *customPlotRx = this->userInterface_->NetUsageGraphRx;
    auto *customPlotTx = this->userInterface_->NetUsageGraphTx;

    customPlotRx->axisRect()->setupFullAxesBox();
    customPlotRx->yAxis->setRange(0.0, 10000.0);
    customPlotRx->yAxis->setLabel("Bandwidth, kib/sec");
    customPlotRx->xAxis->setLabel("Time, sec");

    customPlotTx->axisRect()->setupFullAxesBox();
    customPlotTx->yAxis->setRange(0.0, 10000.0);
    customPlotTx->yAxis->setLabel("Bandwidth, kib/sec");
    customPlotTx->xAxis->setLabel("Time, sec");

    this->titleRx_     = new QCPTextElement(customPlotRx);
    this->subLayoutRx_ = new QCPLayoutGrid;

    this->titleTx_     = new QCPTextElement(customPlotTx);
    this->subLayoutTx_ = new QCPLayoutGrid;

    this->titleRx_->setText("Network Usage Graph (rx)");
    this->titleRx_->setFont(QFont("sans", 12, QFont::Bold));

    this->titleTx_->setText("Network Usage Graph (tx)");
    this->titleTx_->setFont(QFont("sans", 12, QFont::Bold));

    customPlotTx->plotLayout()->insertRow(0);
    customPlotTx->legend->setVisible(true);

    customPlotRx->plotLayout()->insertRow(0);
    customPlotRx->legend->setVisible(true);

    customPlotRx->plotLayout()->addElement(0, 0, this->titleRx_);
    customPlotRx->plotLayout()->addElement(2, 0, this->subLayoutRx_);

    customPlotTx->plotLayout()->addElement(0, 0, this->titleTx_);
    customPlotTx->plotLayout()->addElement(2, 0, this->subLayoutTx_);

    for (auto i = 0; i != this->NetUsage_.size(); ++i)
    {
        customPlotTx->addGraph();
        customPlotTx->graph(i)->setName(this->NetUsage_[i].ifName_);

        customPlotRx->addGraph();
        customPlotRx->graph(i)->setName(this->NetUsage_[i].ifName_);

        switch (i)
        {
            case 0:
                customPlotTx->graph(i)->setPen(QPen(QColor(0, 0, 255)));
                customPlotRx->graph(i)->setPen(QPen(QColor(0, 0, 255)));
                break;

            case 1:
                customPlotTx->graph(i)->setPen(QPen(QColor(0, 255, 0)));
                customPlotRx->graph(i)->setPen(QPen(QColor(0, 255, 0)));
                break;

            case 2:
                customPlotTx->graph(i)->setPen(QPen(QColor(255, 0, 0)));
                customPlotRx->graph(i)->setPen(QPen(QColor(255, 0, 0)));
                break;

            case 3:
                customPlotTx->graph(i)->setPen(QPen(QColor(150, 0, 255)));
                customPlotRx->graph(i)->setPen(QPen(QColor(150, 0, 255)));
                break;

            case 4:
                customPlotTx->graph(i)->setPen(QPen(QColor(0, 150, 255)));
                customPlotRx->graph(i)->setPen(QPen(QColor(0, 150, 255)));
                break;

            case 5:
                customPlotTx->graph(i)->setPen(QPen(QColor(255, 0, 150)));
                customPlotRx->graph(i)->setPen(QPen(QColor(255, 0, 150)));
                break;

            case 6:
                customPlotTx->graph(i)->setPen(QPen(QColor(255, 200, 200)));
                customPlotRx->graph(i)->setPen(QPen(QColor(255, 200, 200)));
                break;

            case 7:
                customPlotTx->graph(i)->setPen(QPen(QColor(10, 250, 200)));
                customPlotRx->graph(i)->setPen(QPen(QColor(10, 250, 200)));
                break;

            default:
                customPlotTx->graph(i)->setPen(QPen(QColor(0, 0, 0)));
                customPlotRx->graph(i)->setPen(QPen(QColor(0, 0, 0)));
                break;
        }

    }

    this->subLayoutRx_->setMargins(QMargins(5, 0, 1, 5));
    this->subLayoutRx_->addElement(0, 0, customPlotRx->legend);

    this->subLayoutTx_->setMargins(QMargins(5, 0, 1, 5));
    this->subLayoutTx_->addElement(0, 0, customPlotTx->legend);


    customPlotRx->legend->setFillOrder(QCPLegend::foColumnsFirst);
    customPlotRx->plotLayout()->setRowStretchFactor(2, 0.1);

    customPlotTx->legend->setFillOrder(QCPLegend::foColumnsFirst);
    customPlotTx->plotLayout()->setRowStretchFactor(2, 0.1);
}

void NetMonitor::connectSignalSlot()
{
    QObject::connect(this->userInterface_->NetUsageGraphRx->xAxis, SIGNAL(rangeChanged(QCPRange)),
            this->userInterface_->NetUsageGraphRx->xAxis2, SLOT(setRange(QCPRange)));

    QObject::connect(this->userInterface_->NetUsageGraphRx->yAxis, SIGNAL(rangeChanged(QCPRange)),
            this->userInterface_->NetUsageGraphRx->yAxis2, SLOT(setRange(QCPRange)));

    QObject::connect(this->userInterface_->NetUsageGraphTx->xAxis, SIGNAL(rangeChanged(QCPRange)),
            this->userInterface_->NetUsageGraphTx->xAxis2, SLOT(setRange(QCPRange)));

    QObject::connect(this->userInterface_->NetUsageGraphTx->yAxis, SIGNAL(rangeChanged(QCPRange)),
            this->userInterface_->NetUsageGraphTx->yAxis2, SLOT(setRange(QCPRange)));
}

void NetMonitor::netStatGet()
{
    auto i = 0;
    struct ifaddrs *ifaddr, *ifa;

    if (getifaddrs(&ifaddr) == -1)
    {
        throw "getifaddrs";
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        int family = ifa->ifa_addr->sa_family;
        if (family == AF_PACKET && ifa->ifa_data != NULL)
        {
            struct rtnl_link_stats *stats = (struct rtnl_link_stats *)ifa->ifa_data;

            this->NetUsage_[i].txBandwidth_ = (stats->tx_bytes - this->NetUsage_[i].txBytes_) / 1024.0;
            this->NetUsage_[i].txBytes_     = stats->tx_bytes;

            this->NetUsage_[i].rxBandwidth_ = (stats->rx_bytes - this->NetUsage_[i].rxBytes_) / 1024.0;
            this->NetUsage_[i].rxBytes_     = stats->rx_bytes;
            i++;
        }
    }
    freeifaddrs(ifaddr);
}
