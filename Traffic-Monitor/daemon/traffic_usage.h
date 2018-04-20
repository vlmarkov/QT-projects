#ifndef _TRAFFIC_USAGE_H_
#define _TRAFFIC_USAGE_H_

class TrafficUsage
{
    public:
        TrafficUsage();
        TrafficUsage(const char* _ifName,
                     const long _rxBytes,
                     const long _txBytes);
        TrafficUsage(const TrafficUsage& rhs);
        ~TrafficUsage();

        char ifName[32];
        long rxBytes;
        long txBytes;
};

#endif // _TRAFFIC_USAGE_H_
