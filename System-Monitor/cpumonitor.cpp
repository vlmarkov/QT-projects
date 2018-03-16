#include "cpumonitor.h"

#include <fstream>
#include <sstream>

CpuData::CpuData() { }

CpuUsage::CpuUsage() { }

CpuMonitor::CpuMonitor(QObject* parent) : QObject(parent)
{
    this->getCpuUsage();
}

CpuMonitor::~CpuMonitor() { }

Q_INVOKABLE void CpuMonitor::getCpuUsage()
{
    static QVector<CpuData> cpuDataPrev;

    QVector<CpuData> cpuDataNext;

    std::ifstream fileStat("/proc/stat");
    if (fileStat.is_open())
    {
        std::string line;

        const std::string STR_CPU("cpu");
        const std::size_t LEN_STR_CPU = STR_CPU.size();
        const std::string STR_TOT("tot");

        while (std::getline(fileStat, line))
        {
            // Cpu stats line found
            if (!line.compare(0, LEN_STR_CPU, STR_CPU))
            {
                std::istringstream ss(line);

                // Stores entry
                cpuDataNext.push_back(CpuData());
                CpuData& cpuData = cpuDataNext.back();

                // Reads cpu label
                ss >> cpuData.cpu;

                // Removes "cpu" from the label when it's a processor number
                if (cpuData.cpu.size() > LEN_STR_CPU)
                {
                    cpuData.cpu.erase(0, LEN_STR_CPU);
                }
                // Replaces "cpu" with "tot" when it's total values
                else
                {
                    cpuData.cpu = STR_TOT;
                }

                // Reads times
                for (auto i = 0; i < NUM_CPU_STATES; ++i)
                {
                    ss >> cpuData.times[i];
                }
            }
        }

        if (this->cpuUsage_.size() == 0)
        {
            this->cpuUsage_ = QVector<CpuUsage>(cpuDataNext.size() - SKIP_COMMON_CPU_STAT);

            for (auto i = 0; i < this->cpuUsage_.size(); ++i)
            {
                this->cpuUsage_[i].name = QString("core " + QString::number(i));
            }
        }

        this->computeUsage_(cpuDataPrev, cpuDataNext);
        cpuDataPrev = cpuDataNext;
    }
    else
    {
        throw QString("Can't read /proc/stat/");
    }
}

Q_INVOKABLE int CpuMonitor::getCores()
{
    return this->cpuUsage_.size();
}

Q_INVOKABLE QString CpuMonitor::getCoreName(const int i)
{
    return this->cpuUsage_[i].name;
}

Q_INVOKABLE double CpuMonitor::getCoreUsage(const int i)
{
    return this->cpuUsage_[i].usage;
}

void CpuMonitor::computeUsage_(const QVector<CpuData>& cpuDataPrev,
                               const QVector<CpuData>& cpuDataNext)
{
    for (auto i = SKIP_COMMON_CPU_STAT; i < cpuDataPrev.size(); ++i)
    {
        auto& _cpuDataPrev = cpuDataPrev[i];
        auto& _cpuDataNext = cpuDataNext[i];

        auto activeTime = static_cast<double>
                (this->getActiveTime_(_cpuDataNext) - this->getActiveTime_(_cpuDataPrev));
        auto idleTime = static_cast<double>
                (this->getIdleTime_(_cpuDataNext) - this->getIdleTime_(_cpuDataPrev));
        auto totalTime = activeTime + idleTime;
        auto usage = (100.f * activeTime / totalTime);

        this->cpuUsage_[i - SKIP_COMMON_CPU_STAT].usage = usage;
    }
}

size_t CpuMonitor::getIdleTime_(const CpuData& cpuData)
{
    return (cpuData.times[S_IDLE] + cpuData.times[S_IOWAIT]);
}

size_t CpuMonitor::getActiveTime_(const CpuData& cpuData)
{
    return (cpuData.times[S_USER]    + cpuData.times[S_NICE]  +
            cpuData.times[S_SYSTEM]  + cpuData.times[S_IRQ]   +
            cpuData.times[S_SOFTIRQ] + cpuData.times[S_STEAL] +
            cpuData.times[S_GUEST]   + cpuData.times[S_GUEST_NICE]);
}
