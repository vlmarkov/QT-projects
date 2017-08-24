#ifndef _NET_MONITOR_H_
#define _NET_MONITOR_H_

#include "abstractdrawer.h"
#include "abstractsystemmonitor.h"
#include "ui_mainwindow.h"

#include <vector>

class NetUsage
{
    public:
        NetUsage(QString name, QString addr, long txBytes, long rxBytes,  double txBandwidth, double  rxBandwidth) :
            ifName_(name), ipAddr_(addr), txBytes_(txBytes), rxBytes_(rxBytes), txBandwidth_(txBandwidth), rxBandwidth_(rxBandwidth) {}

        QString ifName_;
        QString ipAddr_;
        long    txBytes_;
        long    rxBytes_;
        double  txBandwidth_;
        double  rxBandwidth_;
};

class NetMonitor : public AbstractSystemMonitor
{
    public:
        NetMonitor(Ui::MainWindow* ui);
        ~NetMonitor();

        void hwInfoGet();
        void hwInfoShow();
        void hwUsageGather(bool activate);
        void hwUsageShow();

    public slots:
        void hwUsageShowRealTime();

    private:
        Ui::MainWindow* userInterface_;
        AbstractDrawer* drawerRx_;
        AbstractDrawer* drawerTx_;

        std::vector<NetUsage> NetUsage_;

        void createGraph();
        void netStatGet();
};

#endif /* _NET_MONITOR_H_ */
