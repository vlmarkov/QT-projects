#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //AbstractSystemMonitor* cpuMonitor = new CpuMonitor;

    //cpuMonitor->hwInfoGet();
    //cpuMonitor->hwInfoShow();
    //cpuMonitor->hwUsageGather(true);
    //cpuMonitor->hwUsageShow();



    w.show();

    //delete cpuMonitor;

    return a.exec();
}
