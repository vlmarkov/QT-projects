#ifndef _ABSTRACT_SYSTEM_MONITOR_HPP_
#define _ABSTRACT_SYSTEM_MONITOR_HPP_


const int Kib = 1024;
const int Mib = Kib * 1024;
const int Gib = Mib * 1024;

class AbstractSystemMonitor
{
    public:
        AbstractSystemMonitor() {}
        virtual ~AbstractSystemMonitor() {}

        virtual void hwInfoGet() = 0;
        virtual void hwInfoShow() = 0;
        virtual void hwUsageGather(bool activate) = 0;
        virtual void hwUsageShow() = 0;
};


#endif /* _ABSTRACT_SYSTEM_MONITOR_HPP_ */
