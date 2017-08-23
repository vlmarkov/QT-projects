#include "netmonitor.h"

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

    this->titleRx_       = new QCPTextElement(this->userInterface_->NetUsageGraphRx);
    this->titleTx_       = new QCPTextElement(this->userInterface_->NetUsageGraphTx);

    this->subLayoutRx_   = new QCPLayoutGrid;
    this->subLayoutTx_   = new QCPLayoutGrid;
}

NetMonitor::~NetMonitor()
{
    delete this->titleRx_;;
    delete this->subLayoutRx_;
    delete this->titleTx_;
    delete this->subLayoutTx_;
}

void NetMonitor::hwInfoGet()
{
    struct ifaddrs *ifaddr;
    struct ifaddrs *ifa;

    int family, n;

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
    return;
}

void NetMonitor::hwUsageGather(bool activate)
{
    if (!activate)
        return;

    this->createGraph(this->userInterface_->NetUsageGraphRx,
                      QString("Network Resource Usage (download)"),
                      this->titleRx_,
                      this->subLayoutRx_);

    this->createGraph(this->userInterface_->NetUsageGraphTx,
                      QString("Network Resource Usage (upload)"),
                      this->titleTx_,
                      this->subLayoutTx_);

    this->connectSignalSlot(this->userInterface_->NetUsageGraphRx);
    this->connectSignalSlot(this->userInterface_->NetUsageGraphTx);
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

void NetMonitor::addGraphData(NetUsage     &object,
                          QCustomPlot *customPlot,
                          int          i)
{
    QString ifaceName;

    ifaceName += object.ifName_;
    ifaceName += QString(" (");
    ifaceName += object.ipAddr_;
    ifaceName += QString(")");

    customPlot->addGraph();
    customPlot->graph(i)->setName(ifaceName);

    // TODO color generator
    switch (i)
    {
        case 0:  customPlot->graph(i)->setPen(QPen(QColor(0, 0, 255))); break;
        case 1:  customPlot->graph(i)->setPen(QPen(QColor(0, 255, 0))); break;
        case 2:  customPlot->graph(i)->setPen(QPen(QColor(255, 0, 0))); break;
        case 3:  customPlot->graph(i)->setPen(QPen(QColor(150, 0, 255))); break;
        case 4:  customPlot->graph(i)->setPen(QPen(QColor(0, 150, 255))); break;
        case 5:  customPlot->graph(i)->setPen(QPen(QColor(255, 0, 150))); break;
        case 6:  customPlot->graph(i)->setPen(QPen(QColor(255, 200, 200))); break;
        case 7:  customPlot->graph(i)->setPen(QPen(QColor(10, 250, 200))); break;
        default: customPlot->graph(i)->setPen(QPen(QColor(0, 0, 0))); break;
    }
}

void NetMonitor::createGraph(QCustomPlot    *customPlot,
                             QString         titleText,
                             QCPTextElement *title,
                             QCPLayoutGrid  *subLayout)
{
    customPlot->axisRect()->setupFullAxesBox();
    customPlot->yAxis->setRange(0.0, 10000.0);
    customPlot->yAxis->setLabel("Bandwidth, kib/sec");
    customPlot->xAxis->setLabel("Time, sec");

    title->setText(titleText);
    title->setFont(QFont("sans", 12, QFont::Bold));

    customPlot->plotLayout()->insertRow(0);
    customPlot->legend->setVisible(true);

    customPlot->plotLayout()->addElement(0, 0, title);
    customPlot->plotLayout()->addElement(2, 0, subLayout);

    auto ifaces = this->NetUsage_.size();
    auto i = 0;

    std::for_each(this->NetUsage_.begin(), this->NetUsage_.end(), [&](NetUsage data)
    {
        addGraphData(data, customPlot, i++);
    });


    subLayout->setMargins(QMargins(5, 0, 1, 5));
    subLayout->addElement(0, 0, customPlot->legend);

    customPlot->legend->setWrap(ifaces / 6);
    customPlot->legend->setRowSpacing(1);
    customPlot->legend->setColumnSpacing(3);
    customPlot->legend->setFillOrder(QCPLayoutGrid::FillOrder::foColumnsFirst, true);

    auto rowStretchFactor = 0.2;
    if ((double)(ifaces / 6) > 1.0)
        rowStretchFactor *= (double)(ifaces / 6);

    customPlot->plotLayout()->setRowStretchFactor(2, rowStretchFactor);
}

void NetMonitor::connectSignalSlot(QCustomPlot *customPlot)
{
    QObject::connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)),
                     customPlot->xAxis2, SLOT(setRange(QCPRange)));

    QObject::connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)),
                     customPlot->yAxis2, SLOT(setRange(QCPRange)));
}

void NetMonitor::netStatGet()
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
}
