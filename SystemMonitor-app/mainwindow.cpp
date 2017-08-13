#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "cpumonitor.h"
#include "rammonitor.h"
#include "hddmonitor.h"
#include "gpumonitor.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow)
{
    this->ui_->setupUi(this);

    this->cpuMonitor_ = new CpuMonitor(this->ui_);
    this->ramMonitor_ = new RamMonitor(this->ui_);
    this->hddMonitor_ = new HddMonitor(this->ui_);
    this->gpuMonitor_ = new GpuMonitor(this->ui_);

    this->cpuMonitor_->hwInfoGet();
    this->cpuMonitor_->hwInfoShow();
    this->cpuMonitor_->hwUsageGather(true);

    this->ramMonitor_->hwInfoGet();
    this->ramMonitor_->hwInfoShow();
    //this->ramMonitor_->hwUsageGather(true);

    this->hddMonitor_->hwInfoGet();
    this->hddMonitor_->hwInfoShow();
    //this->hddMonitor_->hwUsageGather(true);

    this->gpuMonitor_->hwInfoGet();
    this->gpuMonitor_->hwInfoShow();
    //this->gpuMonitor_->hwUsageGather(true);

    this->createTimer();
    this->connectTimerSlot();
    this->startTimer();
}

MainWindow::~MainWindow()
{
    delete cpuMonitor_;
    delete ramMonitor_;
    delete hddMonitor_;
    delete gpuMonitor_;

    delete timer_;
    delete ui_;
}

void MainWindow::createTimer()
{
    this->timer_ = new QTimer(this);
}

void MainWindow::startTimer()
{
    this->timer_->start(0);
}

void MainWindow::connectTimerSlot()
{
    connect(this->timer_, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
}

void MainWindow::realtimeDataSlot()
{
    this->cpuMonitor_->hwUsageShow();
}
