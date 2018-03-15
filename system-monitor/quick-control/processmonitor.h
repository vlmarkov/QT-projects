#ifndef PROCESSMONITOR_H
#define PROCESSMONITOR_H

#include <QObject>
#include <QString>

class ProcessInfo
{
    public:
        ProcessInfo(QString _pid, QString _name);
        QString pid;
        QString name;
};

class ProcessMonitor : public QObject
{
    Q_OBJECT
    public:
        explicit ProcessMonitor(QObject* parent = 0);
        ~ProcessMonitor();

        void getProcesses();

        Q_INVOKABLE int getProccessNumbers();
        Q_INVOKABLE QString getProccessPid(const int i);
        Q_INVOKABLE QString getProccessName(const int i);
        Q_INVOKABLE void killProcces(const QString pid);

    private:
        std::vector<ProcessInfo> processes_;
};

#endif // PROCESSMONITOR_H
