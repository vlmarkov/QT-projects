#ifndef _TRAFFIC_STORAGE_H_
#define _TRAFFIC_STORAGE_H_

#include <QVector>
#include <string>
#include <fstream>

#include <boost/filesystem.hpp>

#include "traffic_usage.h"

class TrafficStorage
{
    public:
        TrafficStorage();
        ~TrafficStorage();

        QVector<TrafficUsage> read();
        void write(QVector<TrafficUsage>& rhs);

    private:
        std::ifstream           ifstream_;
        std::ofstream           ofstream_;

        std::string             file_;
        boost::filesystem::path path_;
};

#endif // _TRAFFIC_STORAGE_H_
