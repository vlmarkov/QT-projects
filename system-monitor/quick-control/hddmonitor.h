#ifndef HDDMONITOR_H
#define HDDMONITOR_H

#include <QObject>

class HddMonitor : public QObject
{
    Q_OBJECT
    public:
        explicit HddMonitor(QObject* parent = 0);
        ~HddMonitor();

        void hwInfoGet(void);
        void hwInfoShow(void);
        void hwUsageGather(bool activate);
        void hwUsageShow(void);

        Q_INVOKABLE double getFreeSpace();
        Q_INVOKABLE double getUseSpace();
        Q_INVOKABLE QString getUseSpaceStr();
        Q_INVOKABLE QString getFreeSpaceStr();
        Q_INVOKABLE QString getSpaceStr();

    private:
        unsigned long blocksSize_;  // The file system block size
        unsigned long blocksTotal_; // The total number of blocks on the file system in units of f_frsize
        unsigned long blocksFree_;  // The number of free blocks available to non-privileged process
};

#endif // HDDMONITOR_H
