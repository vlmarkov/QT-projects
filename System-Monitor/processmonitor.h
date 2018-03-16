#ifndef PROCESSMONITOR_H
#define PROCESSMONITOR_H

#include <QObject>
#include <QString>
#include <QVector>

class ProcessInfo
{
    public:
        ProcessInfo();
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
        QVector<ProcessInfo> processes_;
};

#endif // PROCESSMONITOR_H
