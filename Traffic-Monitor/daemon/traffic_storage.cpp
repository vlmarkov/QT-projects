#include "traffic_storage.h"

#include <ctime>
#include <cstring>
#include <fstream>

#include <QString>

#include <boost/filesystem.hpp>

TrafficStorage::TrafficStorage()
{
    auto now = std::time(nullptr);
    auto* time = std::localtime(&now);

    char tmp[256];

    sprintf(tmp, "%d/%d", time->tm_year + 1900, time->tm_mon + 1);

    boost::filesystem::create_directories(boost::filesystem::path(tmp));

    sprintf(tmp, "%d/%d/%d.bin", time->tm_year + 1900, time->tm_mon + 1, time->tm_mday);

    this->file_ = tmp;
}

TrafficStorage::~TrafficStorage()
{
    ;
}

QVector<TrafficUsage> TrafficStorage::read()
{
    std::ifstream file;

    file.open(this->file_, std::ios::in | std::ios::binary);
    if (!file.is_open())
    {
        return {};
    }

    auto begin = file.tellg();
    file.seekg(0, std::ios::end);
    auto end = file.tellg();
    file.seekg(0);

    QVector<TrafficUsage> v;

    for (auto i = 0; i < (end - begin) / sizeof(TrafficUsage); ++i)
    {
        TrafficUsage usage;

        file.read(reinterpret_cast<char*>(&usage), sizeof(TrafficUsage));

        if (file.bad()|| file.fail() ||
           (file.gcount() != sizeof(TrafficUsage)))
        {
            throw QString("Can't read data");
        }

        v.push_back(usage);
    }

    file.close();

    return v;
}

void TrafficStorage::write(QVector<TrafficUsage>& rhs)
{
    std::ofstream file;

    file.open(this->file_, std::fstream::out | std::fstream::binary);
    if (!file.is_open())
    {
        throw QString("Can't open file (for write)");
    }

    for (auto iter : rhs)
    {
        file.write(reinterpret_cast<const char*>(&iter), sizeof(iter));
        if (file.bad() || file.fail())
        {
            throw QString("Can't write data");
        }
    }

    file.close();
}
