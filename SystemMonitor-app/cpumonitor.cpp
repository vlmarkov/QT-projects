#include "cpumonitor.h"

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
    this->userInterface_ = ui;
}

CpuMonitor::~CpuMonitor()
{
    // TODO
}

void CpuMonitor::hwInfoGet()
{
    struct cpu_raw_data_t raw;
    struct cpu_id_t data;

    try {     
        if (!cpuid_present())
            throw "Your CPU doesn't support CPUID!";

        if (cpuid_get_raw_data(&raw) < 0)
            throw "Can't get the CPUID raw data";

        if (cpu_identify(&raw, &data) < 0)
            throw "CPU identification failed";

#if __x86_64__
        this->arch_        = "x86-64" ;
#else
        this->arch_        = "x86" ;
#endif
        this->vendor_      = data.vendor_str;
        this->codeName_    = data.cpu_codename;
        this->brandName_   = data.brand_str;
        this->l1cache_     = data.l1_data_cache;
        this->l2cache_     = data.l2_cache;
        this->hwCoresNum_  = data.num_cores;
        this->logcoresNum_ = data.num_logical_cpus;

        this->readCpuFrequencyUsage("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq");

    } catch (std::string err) {
        std::cerr << err << std::endl;
        exit(1); // TODO
    }
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
    this->connectSignalSlot();
}

void CpuMonitor::hwUsageShow()
{
    static QTime time(QTime::currentTime());

    // Calculate two new data points:
    double key = time.elapsed()/1000.0;
    static double lastPointKey = 0;

    if (key-lastPointKey > 0.2) // Every 200 ms
    {
        this->readCpuFrequencyUsage("/proc/cpuinfo");
        for (auto i = 0; i < this->hwCoresNum_; ++i) {
            this->userInterface_->CpuUsageGraph->graph(i)->addData(key,
                this->currFreqUsage_[i]);
        }
        lastPointKey = key;
    }

    this->userInterface_->CpuUsageGraph->xAxis->setRange(key, 8, Qt::AlignRight);
    this->userInterface_->CpuUsageGraph->replot();

    this->currFreqUsage_.clear();
}

void CpuMonitor::createGraph()
{
    auto minFrequency = 0;
    auto maxFrequency = this->maxFrequency_ + 1000;

    this->userInterface_->CpuUsageGraph->axisRect()->setupFullAxesBox();
    this->userInterface_->CpuUsageGraph->yAxis->setRange(minFrequency, maxFrequency);
    this->userInterface_->CpuUsageGraph->yAxis->setLabel("Frequency, Mhz");
    this->userInterface_->CpuUsageGraph->xAxis->setLabel("Time, sec");

    // TODO
    for (auto i = 0; i != this->hwCoresNum_; ++i) {
        this->userInterface_->CpuUsageGraph->addGraph();
        switch (i)
        {
            case 0: this->userInterface_->CpuUsageGraph->graph(i)->setPen(QPen(QColor(0, 0, 255))); break;
            case 1: this->userInterface_->CpuUsageGraph->graph(i)->setPen(QPen(QColor(0, 255, 0))); break;
            case 2: this->userInterface_->CpuUsageGraph->graph(i)->setPen(QPen(QColor(255, 0, 0))); break;
            case 3: this->userInterface_->CpuUsageGraph->graph(i)->setPen(QPen(QColor(150, 0, 255))); break;
            case 4: this->userInterface_->CpuUsageGraph->graph(i)->setPen(QPen(QColor(0, 150, 255))); break;
            case 5: this->userInterface_->CpuUsageGraph->graph(i)->setPen(QPen(QColor(255, 0, 150))); break;
            case 6: this->userInterface_->CpuUsageGraph->graph(i)->setPen(QPen(QColor(255, 200, 200))); break;
            case 7: this->userInterface_->CpuUsageGraph->graph(i)->setPen(QPen(QColor(10, 250, 200))); break;
            default: this->userInterface_->CpuUsageGraph->graph(i)->setPen(QPen(QColor(0, 0, 0))); break;
        }
    }
}

void CpuMonitor::connectSignalSlot()
{
    QObject::connect(this->userInterface_->CpuUsageGraph->xAxis, SIGNAL(rangeChanged(QCPRange)),
            this->userInterface_->CpuUsageGraph->xAxis2, SLOT(setRange(QCPRange)));

    QObject::connect(this->userInterface_->CpuUsageGraph->yAxis, SIGNAL(rangeChanged(QCPRange)),
            this->userInterface_->CpuUsageGraph->yAxis2, SLOT(setRange(QCPRange)));
}

void CpuMonitor::readCpuFrequencyUsage(const std::string sysPath)
{
    if (sysPath.size() == 0)
        throw "Not valid system path";

    std::ifstream sysFile(sysPath);
    if (!sysFile.is_open())
        throw std::string("Can't read" + std::string(sysPath));

    std::string line;
    if (sysPath.compare("/proc/cpuinfo") == 0) {
        while(getline(sysFile, line)) {
            this->parseCpuFrequencyUsage(line);
        }
    } else {
        getline(sysFile, line);
        this->maxFrequency_ = stoi(line) / 1000;
    }
    sysFile.close();
}

void CpuMonitor::parseCpuFrequencyUsage(std::string str)
{
    if (str.find("MHz") == std::string::npos)
        return;

    std::size_t found = str.find(":");
    if (found == std::string::npos)
        return;

    for (auto i = found + 1; i < str.size(); ++i) {
        if (str[i] != ' ') {
            found = i;
            break;
        }
    }
    str.erase(str.begin(), str.begin() + found);
    this->currFreqUsage_.push_back(stod(str));
}
