#include "traffic_usage_reader.h"

#include <iostream>
#include <fstream>

TrafficUsageReader::TrafficUsageReader(QObject* parent) : QObject(parent)
{
    QVector<DateTraffic> v;
    QStringList trafficUsageFiles;

    this->gatherFiles_(boost::filesystem::path("../../history"), trafficUsageFiles);

    for (auto i: trafficUsageFiles)
    {
        this->readFromFile_(v, i.toStdString().c_str(), this->fileNameToDate_(i).toStdString().c_str());
    }

    this->setDatesHistory_(v);
    this->setDevices_(v);
}

TrafficUsageReader::~TrafficUsageReader()
{
    ;
}

QString TrafficUsageReader::fileNameToDate_(QString string)
{
    QStringList list = string.split(QRegExp("[/]"), QString::SkipEmptyParts);
    QString date;

    for (auto i = 3; i < list.size() - 1; ++i)
    {
        date += QString(list[i].toStdString().c_str() + QString("."));

    }

    list = list[list.size() - 1].split(QRegExp("[.]"), QString::SkipEmptyParts);
    date += QString(list[0].toStdString().c_str());

    return date;
}

void TrafficUsageReader::gatherFiles_(boost::filesystem::path dir, QStringList& list)
{
    using namespace boost::filesystem;

    if (is_directory(dir))
    {
        for (auto& entry : boost::make_iterator_range(directory_iterator(dir), {}))
        {
            if (is_directory(entry))
            {
                this->gatherFiles_(entry, list);
            }
            else
            {
                list.push_back(QString(entry.path().string().c_str()));
            }
        }
    }
}

void TrafficUsageReader::setDatesHistory_(const QVector<DateTraffic>& v)
{
    for (auto i: v)
    {
        auto newDate = i._date;
        auto it = std::find(this->dates_.begin(), this->dates_.end(), newDate);
        if (it == this->dates_.end())
        {
            this->dates_.push_back(newDate);
        }
    }
}

void TrafficUsageReader::setDevices_(const QVector<DateTraffic>& v)
{
    for (auto i: v)
    {
        auto newDevice = i._device;
        auto it = std::find_if(this->devices_.begin(), this->devices_.end(), [&newDevice](const DeviceTraffic& deviceTraffic)
        {
            return deviceTraffic._device == newDevice;
        });

        if (it == this->devices_.end())
        {
            this->devices_.push_back(newDevice);
            this->devices_.back().addDateTraffic(i);
        }
        else
        {
            it->addDateTraffic(i);
        }
    }

    for (auto i = 0; i < this->devices_.size(); ++i)
    {
        this->devices_[i].convertUsageTo(gibStr, gibInt);
    }
}

void TrafficUsageReader::readFromFile_(QVector<DateTraffic>& v, const char* path, const QString& date)
{
    std::ifstream file;

    file.open(path, std::ios::in | std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Can't open file" << std::endl;
        return;
    }

    auto begin = file.tellg();
    file.seekg(0, std::ios::end);
    auto end = file.tellg();
    file.seekg(0);

    for (auto i = 0; i < (end - begin) / sizeof(trafficRawData_t); ++i)
    {
        trafficRawData_t rawData;

        file.read(reinterpret_cast<char*>(&rawData), sizeof(rawData));
        if (file.bad()|| file.fail() ||
           (file.gcount() != sizeof(rawData)))
        {
            throw std::string("Can't read data");
        }
        v.push_back(DateTraffic(date, rawData.ifName, rawData.rxBytes, rawData.txBytes));
    }

    file.close();
}

Q_INVOKABLE int TrafficUsageReader::getDevices()
{
    return this->devices_.size();
}

Q_INVOKABLE int TrafficUsageReader::getHistorySize()
{
    return this->dates_.size();
}

Q_INVOKABLE QStringList TrafficUsageReader::getHistory()
{
    return this->dates_;
}

Q_INVOKABLE double TrafficUsageReader::getDevicesMaxTx()
{
    double max = 0.0;

    for (auto i = 0; i < this->devices_.size(); ++i)
    {
        for (auto j = 0; j < this->devices_[i]._history.size(); ++j)
        {
            if (max < this->devices_[i]._history[j]._txBytes)
            {
                max = this->devices_[i]._history[j]._txBytes;
            }
        }
    }

    return ((max * 1.1) / (gibInt));
}

Q_INVOKABLE double TrafficUsageReader::getDevicesMaxRx()
{
    double max = 0.0;

    for (auto i = 0; i < this->devices_.size(); ++i)
    {
        for (auto j = 0; j < this->devices_[i]._history.size(); ++j)
        {
            if (max < this->devices_[i]._history[j]._rxBytes)
            {
                max = this->devices_[i]._history[j]._rxBytes;
            }
        }
    }

    return ((max * 1.1) / (gibInt));
}

Q_INVOKABLE QString TrafficUsageReader::getDevicesLabelTx()
{
    return gibStr;
}

Q_INVOKABLE QString TrafficUsageReader::getDevicesLabelRx()
{
    return gibStr;
}

Q_INVOKABLE QString TrafficUsageReader::getDeviceName(const int device)
{
    // TODO: errors check
    return this->devices_[device]._device;
}

Q_INVOKABLE double TrafficUsageReader::getDeviceTxTotal(const int device)
{
    // TODO: errors check
    return this->devices_[device]._txTotal;
}

Q_INVOKABLE double TrafficUsageReader::getDeviceRxTotal(const int device)
{
    // TODO: errors check
    return this->devices_[device]._rxTotal;
}

Q_INVOKABLE double TrafficUsageReader::getDeviceTxPerDate(const int device, const int date)
{
    // TODO: errors check
    return (this->devices_[device]._history[date]._txBytes / (double)gibInt);
}

Q_INVOKABLE double TrafficUsageReader::getDeviceRxPerDate(const int device, const int date)
{
    // TODO: errors check
    return (this->devices_[device]._history[date]._rxBytes / (double)gibInt);
}
