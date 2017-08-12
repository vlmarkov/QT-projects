#ifndef _HDD_MONITOR_H_
#define _HDD_MONITOR_H_

#include "abstractsystemmonitor.h"
#include "ui_mainwindow.h"


class HddMonitor : public AbstractSystemMonitor, QObject
{
    public:
        HddMonitor(Ui::MainWindow* ui);
        ~HddMonitor();

        void hwInfoGet();
        void hwInfoShow();
        void hwUsageGather(bool activate);
        void hwUsageShow();

    private:
        Ui::MainWindow* userInterface_;

        unsigned long blocksSize_;  // The file system block size
        unsigned long blocksTotal_; // The total number of blocks on the file system in units of f_frsize
        unsigned long blocksFree_;  // The number of free blocks available to non-privileged process
};

#endif /* _HDD_MONITOR_H_ */

