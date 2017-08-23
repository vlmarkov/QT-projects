#ifndef _RAM_MONITOR_H_
#define _RAM_MONITOR_H_

#include "abstractsystemmonitor.h"
#include "ui_mainwindow.h"


class RamMonitor : public AbstractSystemMonitor
{
    public:
        RamMonitor(Ui::MainWindow* ui);
        ~RamMonitor();

        void hwInfoGet();
        void hwInfoShow();
        void hwUsageGather(bool activate);
        void hwUsageShow();

    private:
        Ui::MainWindow* userInterface_;

        QString totalRam_;  // Total usable main memory size
        QString freeRam_;   // Available memory size
        QString sharedRam_; // Amount of shared memory
        QString bufferRam_; // Memory used by buffers
        QString totalSwap_; // Total swap space size
        QString freeSwap_;  // Swap space still available
};

#endif /* _RAM_MONITOR_H_ */
