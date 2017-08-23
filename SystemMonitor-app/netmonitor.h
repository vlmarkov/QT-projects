#ifndef _NET_MONITOR_H_
#define _NET_MONITOR_H_

#include "abstractsystemmonitor.h"
#include "ui_mainwindow.h"

#include <vector>

class NetUsage
{
    public:
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
        std::vector<NetUsage> NetUsage_;
        Ui::MainWindow* userInterface_;

        QCPTextElement *titleTx_;
        QCPLayoutGrid  *subLayoutTx_;
        QCPTextElement *titleRx_;
        QCPLayoutGrid  *subLayoutRx_;

        void createGraph(QCustomPlot    *customPlot,
                         QString         titleText,
                         QCPTextElement *title,
                         QCPLayoutGrid  *subLayout);

        void addGraphData(NetUsage    &object,
                          QCustomPlot *customPlot,
                          int          i);

        void connectSignalSlot(QCustomPlot *customPlot);
        void netStatGet();
};


#endif /* _NET_MONITOR_H_ */

