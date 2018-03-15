#include <QQmlApplicationEngine>

#include <QtWidgets/QApplication>
#include <QtQuick/QQuickView>

#include "hddmonitor.h"
#include "cpumonitor.h"
#include "osmonitor.h"
#include "rammonitor.h"
#include "netmonitor.h"
#include "processmonitor.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    qmlRegisterType<OsMonitor>      ("monitor.os",   1, 0, "OsMonitor");
    qmlRegisterType<HddMonitor>     ("monitor.hdd",  1, 0, "HddMonitor");
    qmlRegisterType<CpuMonitor>     ("monitor.cpu",  1, 0, "CpuMonitor");
    qmlRegisterType<RamMonitor>     ("monitor.ram",  1, 0, "RamMonitor");
    qmlRegisterType<NetMonitor>     ("monitor.net",  1, 0, "NetMonitor");
    qmlRegisterType<ProcessMonitor> ("monitor.proc", 1, 0, "ProcMonitor");

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl("qrc:/main.qml"));

    return app.exec();
}
