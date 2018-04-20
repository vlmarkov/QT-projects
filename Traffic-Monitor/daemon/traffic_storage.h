#ifndef _TRAFFIC_STORAGE_H_
#define _TRAFFIC_STORAGE_H_

#include "traffic_usage.h"

#include <QVector>

#include <string>

class TrafficStorage
{
    public:
        TrafficStorage();
        ~TrafficStorage();

        QVector<TrafficUsage> read();
        void write(QVector<TrafficUsage>& rhs);

    private:
        std::string file_;
};

#endif // _TRAFFIC_STORAGE_H_
