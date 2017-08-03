#ifndef _CPU_MONITOR_HPP_
#define _CPU_MONITOR_HPP_

#include "abstractsystemmonitor.h"

#include "ui_mainwindow.h"

#include <string>

class CpuMonitor : public AbstractSystemMonitor
{
    private:
        std::string cpuCores_;
        std::string modelName_;
        std::string vendorId_;
        std::string flags_;
        std::vector<std::string> cpuFrequency_;

        void hwInfoAsign(std::string line, int lnNum);
        Ui::MainWindow* ui_;

    public:
        CpuMonitor(Ui::MainWindow* ui);
        ~CpuMonitor();

        void hwInfoGet();
        void hwInfoShow();
        void hwUsageGather(bool activate);
        void hwUsageShow();
        void hwUsageShowRealTime();
};


#endif /* _CPU_MONITOR_HPP_ */
