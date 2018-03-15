#include "processmonitor.h"

#include <QDir>

#include <iostream>
#include <fstream>

#include <sys/types.h>
#include <signal.h>

ProcessInfo::ProcessInfo(QString _pid, QString _name) : pid(_pid), name(_name) { }

ProcessMonitor::ProcessMonitor(QObject* parent) : QObject(parent)
{
    this->getProcesses();
}

ProcessMonitor::~ProcessMonitor() { }

void ProcessMonitor::getProcesses()
{
    QDir dir("/proc");

    QStringList list = dir.entryList(QStringList("*"), QDir::AllDirs);

    foreach (QString pid, list)
    {
        if (pid.toInt())
        {
            std::ifstream stream("/proc/" + pid.toStdString() + "/comm");

            std::string name;
            std::getline(stream, name);

            this->processes_.push_back(ProcessInfo(pid, QString(name.c_str())));
        } else {
            continue;
        }
    }
}

Q_INVOKABLE int ProcessMonitor::getProccessNumbers()
{
    return this->processes_.size();
}

Q_INVOKABLE QString ProcessMonitor::getProccessPid(const int i)
{
    return this->processes_[i].pid;
}

Q_INVOKABLE QString ProcessMonitor::getProccessName(const int i)
{
    return this->processes_[i].name;
}

Q_INVOKABLE void ProcessMonitor::killProcces(const QString pid)
{
    kill(pid.toInt(), SIGKILL);
}
