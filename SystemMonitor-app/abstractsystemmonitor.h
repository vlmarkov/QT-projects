#ifndef _ABSTRACT_SYSTEM_MONITOR_HPP_
#define _ABSTRACT_SYSTEM_MONITOR_HPP_


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
