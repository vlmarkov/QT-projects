#ifndef CPUMONITOR_H
#define CPUMONITOR_H

#include <QObject>

#include <chrono>
#include <string>
#include <thread>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

static const int NUM_CPU_STATES = 10;
constexpr int SKIP_COMMON_CPU_STAT = 1;

enum CpuStates {
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
} CpuData;

typedef struct {
    QString name;
    double usage;
} CpuUsage;

class CpuMonitor : public QObject
{
    Q_OBJECT
    public:
        explicit CpuMonitor(QObject* parent = 0);
        ~CpuMonitor();

        Q_INVOKABLE void getCpuUsage();
        Q_INVOKABLE int getCores();
        Q_INVOKABLE QString getCoreName(const int core);
        Q_INVOKABLE double getCoreUsage(const int core);

    private:
        std::vector<CpuUsage> cpuUsage_;

        void computeUsage_(const std::vector<CpuData>& cpuDataPrev,
                           const std::vector<CpuData>& cpuDataNext);
        size_t getIdleTime_(const CpuData& cpuData);
        size_t getActiveTime_(const CpuData& cpuData);
};

#endif // CPUMONITOR_H
