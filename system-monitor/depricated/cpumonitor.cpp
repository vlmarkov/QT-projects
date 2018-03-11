#include "cpumonitor.h"
#include "qcustomdrawer.h"

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>
#include <chrono>
#include <thread>

#include <qcustomplot/qcustomplot.h>
#include <cpuid/libcpuid.h>

CpuMonitor::CpuMonitor(Ui::MainWindow* ui)
{
    this->userInterface_  = ui;
    this->drawerCpuUsage_ = new QCustomDrawer(this->userInterface_->CpuUsageGraph);
}

CpuMonitor::~CpuMonitor()
{
    delete this->drawerCpuUsage_;
}

void CpuMonitor::hwInfoGet()
{
    struct cpu_raw_data_t raw;
    struct cpu_id_t data;

    if (!cpuid_present())
        throw "[CpuMonitor] Your CPU doesn't support CPUID!";

    if (cpuid_get_raw_data(&raw) < 0)
        throw "[CpuMonitor] Can't get the CPUID raw data";

    if (cpu_identify(&raw, &data) < 0)
        throw "[CpuMonitor] CPU identification failed";

    #if __x86_64__
        this->arch_         = "x86-64" ;
    #else
        this->arch_         = "x86" ;
    #endif

    this->vendor_       = data.vendor_str;
    this->codeName_     = data.cpu_codename;
    this->brandName_    = data.brand_str;
    this->l1cache_      = data.l1_data_cache;
    this->l2cache_      = data.l2_cache;
    this->hwCoresNum_   = data.num_cores;
    this->logcoresNum_  = data.num_logical_cpus;
    this->minFrequency_ = 0;
    this->maxFrequency_ = readCpuFrequencyMax();

    // Create cpu-statistic-usage vector
    this->cpuUsage_ = std::vector<long double> (this->hwCoresNum_);
}

void CpuMonitor::hwInfoShow()
{
    this->userInterface_->Arch_txt->setText(this->arch_);
    this->userInterface_->Vendor_txt->setText(this->vendor_);
    this->userInterface_->Model_txt->setText(this->codeName_);
    this->userInterface_->Brand_txt->setText(this->brandName_);

    this->userInterface_->HwCores_txt->setText(QString::number(this->hwCoresNum_));
    this->userInterface_->LogCores_txt->setText(QString::number(this->logcoresNum_));
    this->userInterface_->L1cache_txt->setText(QString::number(this->l1cache_));
    this->userInterface_->L2cache_txt->setText(QString::number(this->l2cache_));
}

void CpuMonitor::hwUsageGather(bool activate)
{
    if (!activate)
        return;

    this->createGraph();

    // Warm-up cpu statistic
    this->readStatsCpu();
}

void CpuMonitor::hwUsageShow()
{
    static QTime time(QTime::currentTime());
    static double lastTimePoint1 = 0;
    static double lastTimePoint2 = 0;

    // Calculate two new data points:
    double timePoint1 = time.elapsed()/1000.0;
    double timePoint2 = time.elapsed()/1000.0;

    if (timePoint1 - lastTimePoint1 > 0.2) {
        auto i = 0;
        for_each(this->cpuUsage_.begin(), this->cpuUsage_.end(), [&](long double usage)
        {
            this->drawerCpuUsage_->setPlotData(timePoint1, usage, i);
            i++;
        });

        lastTimePoint1 = timePoint1;
        this->drawerCpuUsage_->replotCustomPlot(timePoint1, 100.0);
    }

    if (timePoint2 - lastTimePoint2 > 1.0) {
        this->readStatsCpu();
        auto i = 0;
        for_each(this->cpuUsage_.begin(), this->cpuUsage_.end(), [&](long double usage)
        {
            QString name;

            name += QString("core ");
            name += QString::number(i);
            name += QString(" ");
            name += QString::number((int)usage);
            name += QString("%");

            this->drawerCpuUsage_->setPlotName(name, i);
            i++;
        });
        lastTimePoint2 = timePoint2;
    }
}

void CpuMonitor::createGraph()
{
    QString titleText("CPU Resource Usage");
    QString yLabelText("Usage, %");
    QString xLabelText("Time, sec");
    std::pair<double, double> yRange(0.0, 101.0);
    int dataSize = (int)this->cpuUsage_.size();

    this->drawerCpuUsage_->createCustomPlot(titleText, yLabelText, xLabelText, yRange, dataSize, 0.1);
    this->drawerCpuUsage_->connectSignalSlot();

    auto i = 0;
    for_each(this->cpuUsage_.begin(), this->cpuUsage_.end(), [&](long double usage)
    {
        QString name;

        name += QString("core ");
        name += QString::number(i);
        name += QString(" ");
        name += QString::number((int)usage);
        name += QString("%");

        this->drawerCpuUsage_->setPlotName(name, i);
        this->drawerCpuUsage_->setPlotColor(i);
        i++;
    });
}

uint32_t CpuMonitor::readCpuFrequencyMax()
{
    uint32_t maxFrequency = 10000000;
    std::ifstream sysFile("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq", std::ios::in);
    if (sysFile.is_open()) {
        std::string line;
        getline(sysFile, line);
        maxFrequency = stoi(line);
        sysFile.close();
    }
    return maxFrequency / 1000;
}

size_t CpuMonitor::getIdleTime(const CPUData & e)
{
    return	e.times[S_IDLE] + e.times[S_IOWAIT];
}

size_t CpuMonitor::getActiveTime(const CPUData & e)
{
    return	e.times[S_USER]    + e.times[S_NICE]  +
            e.times[S_SYSTEM]  + e.times[S_IRQ]   +
            e.times[S_SOFTIRQ] + e.times[S_STEAL] +
            e.times[S_GUEST]   + e.times[S_GUEST_NICE];
}

void CpuMonitor::readStatsCpu()
{
    std::vector<CPUData> entries;

    std::ifstream fileStat("/proc/stat");
    if (fileStat.is_open()) {
        std::string line;

        const std::string STR_CPU("cpu");
        const std::size_t LEN_STR_CPU = STR_CPU.size();
        const std::string STR_TOT("tot");

        while (std::getline(fileStat, line)) {
            // cpu stats line found
            if (!line.compare(0, LEN_STR_CPU, STR_CPU)) {
                std::istringstream ss(line);

                // store entry
                entries.emplace_back(CPUData());
                CPUData & entry = entries.back();

                // read cpu label
                ss >> entry.cpu;

                // remove "cpu" from the label when it's a processor number
                if (entry.cpu.size() > LEN_STR_CPU) {
                    entry.cpu.erase(0, LEN_STR_CPU);
                }
                // replace "cpu" with "tot" when it's total values
                else {
                    entry.cpu = STR_TOT;
                }

                // read times
                for (int i = 0; i < NUM_CPU_STATES; ++i)
                    ss >> entry.times[i];
            }
        }

        this->PrintStats(this->entries1, entries);
        this->entries1 = entries;
    } else {
        throw "Can't read /proc/stat/";
    }
}

void CpuMonitor::PrintStats(const std::vector<CPUData> & entries1,
                            const std::vector<CPUData> & entries2)
{
    const size_t NUM_ENTRIES = entries1.size();

    for(size_t i = 1; i < NUM_ENTRIES; ++i)
    {
        const CPUData & e1 = entries1[i];
        const CPUData & e2 = entries2[i];

        const float ACTIVE_TIME	= static_cast<float>
                (this->getActiveTime(e2) - this->getActiveTime(e1));

        const float IDLE_TIME	= static_cast<float>
                (this->getIdleTime(e2) - this->getIdleTime(e1));

        const float TOTAL_TIME	= ACTIVE_TIME + IDLE_TIME;

        float usage = (100.f * ACTIVE_TIME / TOTAL_TIME);

        this->cpuUsage_[i - 1] = usage;
    }
}