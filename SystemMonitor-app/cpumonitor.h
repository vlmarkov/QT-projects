#ifndef _CPU_MONITOR_HPP_
#define _CPU_MONITOR_HPP_

#include "abstractsystemmonitor.h"

#include "ui_mainwindow.h"

#include <string>
#include <fstream>

typedef struct {
   long double usage[4];
} cpuUsageStat;

class CpuMonitor : public AbstractSystemMonitor, QObject
{
    private:
        int32_t l1cache_;
        int32_t l2cache_;
        int32_t hwCoresNum_;
        int32_t logcoresNum_;
        int32_t minFrequency_;
        int32_t maxFrequency_;

        QString arch_;
        QString vendor_;
        QString codeName_;
        QString brandName_;

        std::vector<cpuUsageStat> statCpuUsage_;
        std::vector<long double> cpuUsage_;

        Ui::MainWindow* userInterface_;

        void createGraph();
        void connectSignalSlot();

        void parseCpuFrequencyUsage(std::string& str, int coreNumber);
        void readCpuFrequencyUsage();
        uint32_t readCpuFrequencyMax();

    public:
        CpuMonitor(Ui::MainWindow* ui);
        ~CpuMonitor();

        void hwInfoGet();
        void hwInfoShow();
        void hwUsageGather(bool activate);
        void hwUsageShow();

    public slots:
        void hwUsageShowRealTime();
};


#endif /* _CPU_MONITOR_HPP_ */
