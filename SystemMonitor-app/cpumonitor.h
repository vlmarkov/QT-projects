#ifndef _CPU_MONITOR_H_
#define _CPU_MONITOR_H_

#include "abstractsystemmonitor.h"

#include "ui_mainwindow.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

static const int NUM_CPU_STATES = 10;

enum CPUStates {
    S_USER = 0,
    S_NICE,
    S_SYSTEM,
    S_IDLE,
    S_IOWAIT,
    S_IRQ,
    S_SOFTIRQ,
    S_STEAL,
    S_GUEST,
    S_GUEST_NICE
};

typedef struct {
    std::string cpu;
    size_t times[NUM_CPU_STATES];
} CPUData;

class CpuMonitor : public AbstractSystemMonitor
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

        std::vector<long double> cpuUsage_;

        Ui::MainWindow* userInterface_;

        void createGraph();
        void connectSignalSlot();

        void parseCpuFrequencyUsage(std::string& str, int coreNumber);
        void readCpuFrequencyUsage();
        uint32_t readCpuFrequencyMax();

        std::vector<CPUData> entries1;

        void readStatsCpu();
        size_t getIdleTime(const CPUData & e);
        size_t getActiveTime(const CPUData & e);
        void PrintStats(const std::vector<CPUData> & entries1,
                        const std::vector<CPUData> & entries2);

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
