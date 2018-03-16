#ifndef CPUMONITOR_H
#define CPUMONITOR_H

#include <QObject>
#include <QVector>

#include <string>

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

class CpuData
{
    public:
        CpuData();

        std::string cpu;
        size_t times[NUM_CPU_STATES];
};

class CpuUsage
{
    public:
        CpuUsage();

        QString name;
        double usage;
};

class CpuMonitor : public QObject
{
    Q_OBJECT
    public:
        explicit CpuMonitor(QObject* parent = 0);
        ~CpuMonitor();

        Q_INVOKABLE void getCpuUsage();
        Q_INVOKABLE int getCores();
        Q_INVOKABLE QString getCoreName(const int i);
        Q_INVOKABLE double getCoreUsage(const int i);

    private:
        QVector<CpuUsage> cpuUsage_;

        void computeUsage_(const QVector<CpuData>& cpuDataPrev,
                           const QVector<CpuData>& cpuDataNext);
        size_t getIdleTime_(const CpuData& cpuData);
        size_t getActiveTime_(const CpuData& cpuData);
};

#endif // CPUMONITOR_H
