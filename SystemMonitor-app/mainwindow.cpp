#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "cpumonitor.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->cpuMonitor = new CpuMonitor(ui);

    this->cpuMonitor->hwInfoGet();
    this->cpuMonitor->hwInfoShow();

    this->dataTimer = new QTimer(this);

    this->ui->widget->axisRect()->setupFullAxesBox();
    this->ui->widget->yAxis->setRange(0, 10000);
    this->ui->widget->yAxis->setLabel("Frequency, Mhz");
    this->ui->widget->xAxis->setLabel("Time, sec");

    connect(this->ui->widget->xAxis,
            SIGNAL(rangeChanged(QCPRange)),
            this->ui->widget->xAxis2,
            SLOT(setRange(QCPRange)));

    connect(this->ui->widget->yAxis,
            SIGNAL(rangeChanged(QCPRange)),
            this->ui->widget->yAxis2,
            SLOT(setRange(QCPRange)));

    connect(dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    this->dataTimer->start(0);
}

void MainWindow::realtimeDataSlot()
{
    //std::cout << __FUNCTION__ << std::endl;
    this->cpuMonitor->hwUsageShow();
    //std::cout << __FUNCTION__ << std::endl;
/*
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.002) // at most add point every 2 ms
    {
      // add data to lines:
      ui->widget->graph(0)->addData(key, qSin(key)+qrand()/(double)RAND_MAX*1*qSin(key/0.3843));
      lastPointKey = key;
    }
    ui->widget->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->widget->replot();
*/
}

MainWindow::~MainWindow()
{
    delete ui;
    delete cpuMonitor;
    delete dataTimer;
}
