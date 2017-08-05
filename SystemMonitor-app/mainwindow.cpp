#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "cpumonitor.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->cpuMonitor = new CpuMonitor(ui);

    this->cpuMonitor->hwInfoGet();
    this->cpuMonitor->hwInfoShow();
    this->cpuMonitor->hwUsageGather(true);

    this->createTimer();
    this->connectTimerSlot();
    this->startTimer();
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
    this->cpuMonitor->hwUsageShow();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete cpuMonitor;
    delete timer_;
}
