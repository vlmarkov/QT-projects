#include "traffic_storage.h"

#include <ctime>
#include <cstring>

#include <QString>

TrafficStorage::TrafficStorage()
{
    auto now = std::time(nullptr);
    auto* time = std::localtime(&now);

    char tmp[256];

    sprintf(tmp, "%d/%d", time->tm_year + 1900, time->tm_mon + 1);

    this->path_ = tmp;

    sprintf(tmp, "%d/%d/%d.bin", time->tm_year + 1900, time->tm_mon + 1, time->tm_mday);

    this->file_ = tmp;

    boost::filesystem::create_directories(this->path_);
}

TrafficStorage::~TrafficStorage()
{
    ;
}

QVector<TrafficUsage> TrafficStorage::read()
{
    this->ifstream_.open(this->file_, std::ios::in | std::ios::binary);
    if (!this->ifstream_.is_open())
    {
        return {};
    }

    auto begin = this->ifstream_.tellg();
    this->ifstream_.seekg(0, std::ios::end);
    auto end = this->ifstream_.tellg();
    this->ifstream_.seekg(0);

    QVector<TrafficUsage> v;

    for (auto i = 0; i < (end - begin) / sizeof(TrafficUsage); ++i)
    {
        TrafficUsage usage;

        this->ifstream_.read(reinterpret_cast<char*>(&usage), sizeof(TrafficUsage));

        if (this->ofstream_.bad()|| this->ofstream_.fail() ||
            (this->ifstream_.gcount() != sizeof(TrafficUsage)))
        {
            throw QString("Can't read data");
        }

        v.push_back(usage);
    }

    this->ifstream_.close();

    return v;
}

void TrafficStorage::write(QVector<TrafficUsage>& rhs)
{
    this->ofstream_.open(this->file_, std::fstream::out | std::fstream::binary);
    if (!this->ofstream_.is_open())
    {
        throw QString("Can't open file (for write)");
    }

    for (auto iter : rhs)
    {
        //auto tmp = iter;
        this->ofstream_.write(reinterpret_cast<const char*>(&iter), sizeof(iter));
        if (this->ofstream_.bad() || this->ofstream_.fail())
        {
            throw QString("Can't write data");
        }
    }

    this->ofstream_.close();
}
