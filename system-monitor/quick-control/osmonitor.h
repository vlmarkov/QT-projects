#ifndef OSMONITOR_H
#define OSMONITOR_H

#include <QObject>

#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>

class OsMonitor : public QObject
{
    Q_OBJECT
    public:
        explicit OsMonitor(QObject* parent = 0);
        ~OsMonitor();

        void getHwInfo();

        Q_INVOKABLE QString getOsNameStr();
        Q_INVOKABLE QString getHostNameStr();
        Q_INVOKABLE QString getUserNameStr();
        Q_INVOKABLE QString getUpTimeStr();
        Q_INVOKABLE QString getTotalRamStr();
        Q_INVOKABLE QString getCpuNameStr();

    private:
        struct utsname utsname_;
        struct passwd* pw_;
        struct sysinfo info_;
};

#endif // OSMONITOR_H
