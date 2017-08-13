#ifndef _GPU_MONITOR_H_
#define _GPU_MONITOR_H_

#include "abstractsystemmonitor.h"
#include "ui_mainwindow.h"


class GpuMonitor : public AbstractSystemMonitor
{
    public:
        GpuMonitor(Ui::MainWindow* ui);
        ~GpuMonitor();

        void hwInfoGet();
        void hwInfoShow();
        void hwUsageGather(bool activate);
        void hwUsageShow();

    private:
        Ui::MainWindow* userInterface_;

        QString modelName_;
        QString totalMemory_;
};

#endif /* _GPU_MONITOR_H_ */

