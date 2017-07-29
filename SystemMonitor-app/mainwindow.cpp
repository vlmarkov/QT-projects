#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "cpumonitor.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cpuMonitor = new CpuMonitor(ui);

    cpuMonitor->hwInfoGet();
    cpuMonitor->hwInfoShow();
    cpuMonitor->hwUsageShow();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete cpuMonitor;
}
