#ifndef RAMMONITOR_H
#define RAMMONITOR_H

#include <QObject>

class RamMonitor : public QObject
{
    Q_OBJECT
    public:
        explicit RamMonitor(QObject* parent = 0);
        ~RamMonitor();

        void hwInfoGet();

        Q_INVOKABLE void updateHwInfo();

        Q_INVOKABLE double getTotalRam();
        Q_INVOKABLE double getUsedRam();
        Q_INVOKABLE double getFreeRam();
        Q_INVOKABLE double getSharedRam();
        Q_INVOKABLE double getBufferedRam();
        Q_INVOKABLE double getTotalSwap();
        Q_INVOKABLE double getUsedSwap();
        Q_INVOKABLE double getFreeSwap();

    private:
        double totalRam_;  // Total usable main memory size
        double freeRam_;   // Available memory size
        double sharedRam_; // Amount of shared memory
        double bufferRam_; // Memory used by buffers
        double totalSwap_; // Total swap space size
        double freeSwap_;  // Swap space still available
};

#endif // RAMMONITOR_H
