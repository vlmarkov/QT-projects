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

        unsigned long totalRam_;  // Total usable main memory size
        unsigned long freeRam_;   // Available memory size
        unsigned long sharedRam_; // Amount of shared memory
        unsigned long bufferRam_; // Memory used by buffers
        unsigned long totalSwap_; // Total swap space size
        unsigned long freeSwap_;  // Swap space still available
};

#endif /* _RAM_MONITOR_H_ */
