#include "traffic_usage.h"

#include <QString>

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

    snprintf(this->ifName, sizeof(this->ifName), "%s", _ifName);

    this->rxBytes = _rxBytes;
    this->txBytes = _txBytes;
}

TrafficUsage::TrafficUsage(const TrafficUsage& rhs)
{  
    snprintf(this->ifName, sizeof(this->ifName), "%s", rhs.ifName);

    this->rxBytes = rhs.rxBytes;
    this->txBytes = rhs.txBytes;
}
