#ifndef _TRAFFIC_MONITOR_H_
#define _TRAFFIC_MONITOR_H_

#include "traffic_usage.h"

#include <QVector>

class TrafficMonitor
{
    public:
        TrafficMonitor();
        TrafficMonitor(const QVector<TrafficUsage>& rhs);
        ~TrafficMonitor();

        QVector<TrafficUsage> getNetworkUsage();

    private:
        QVector<TrafficUsage> usage_;
        void syncTraffic_(QVector<TrafficUsage>& rhs);
};

#endif // _TRAFFIC_MONITOR_H_
