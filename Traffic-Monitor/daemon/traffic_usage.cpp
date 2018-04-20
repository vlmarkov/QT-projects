#include "traffic_usage.h"

#include <QString>
#include <cstring>

TrafficUsage::TrafficUsage()
{
    ;
}

TrafficUsage::~TrafficUsage()
{
    ;
}

TrafficUsage::TrafficUsage(const char* _ifName,
                           const long _rxBytes,
                           const long _txBytes)
{
    if (!_ifName)
    {
        throw QString("Bad pointer to interface name");
    }

    std::memset(&this->ifName, 0, sizeof(this->ifName));
    if (!std::memcpy(this->ifName, _ifName, sizeof(this->ifName)))
    {
        throw QString("Can't copy interface name");
    }

    this->rxBytes = _rxBytes;
    this->txBytes = _txBytes;
}

TrafficUsage::TrafficUsage(const TrafficUsage& rhs)
{
    std::memset(&this->ifName, 0, sizeof(this->ifName));
    if (!std::memcpy(this->ifName, &rhs.ifName, sizeof(this->ifName)))
    {
        throw QString("Can't copy interface name");
    }

    this->rxBytes = rhs.rxBytes;
    this->txBytes = rhs.txBytes;
}
