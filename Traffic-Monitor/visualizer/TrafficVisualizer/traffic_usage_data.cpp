#include "traffic_usage_data.h"

/* */
DateTraffic::DateTraffic()
{
    ;
}

DateTraffic::DateTraffic(const QString& date,
                         const QString& device,
                         const long rxBytes,
                         const long txBytes) :
    _date(date),
    _device(device),
    _rxBytes(rxBytes),
    _txBytes(txBytes)
{
    ;
}

DateTraffic::~DateTraffic()
{
    ;
}

/* */
DeviceTraffic::DeviceTraffic() { }

DeviceTraffic::DeviceTraffic(const QString& name) :
    _device(name),
    _dataTypeRx(""),
    _dateTypeTx(""),
    _rxTotal(0),
    _txTotal(0)
{
    ;
}

DeviceTraffic::~DeviceTraffic()
{
    ;
}

void DeviceTraffic::addDateTraffic(DateTraffic& dateTraffic)
{
    this->_history.push_back(dateTraffic);
}

void DeviceTraffic::convertUsageTo(const QString& strUsage,
                                   const int&     intUsage)
{
    this->_dataTypeRx = strUsage;
    this->_dateTypeTx = strUsage;

    for (auto i = 0; i < this->_history.size(); ++i)
    {
        this->_rxTotal += this->_history[i]._rxBytes;
        this->_txTotal += this->_history[i]._txBytes;
    }

    this->_rxTotal /= intUsage;
    this->_txTotal /= intUsage;
}
