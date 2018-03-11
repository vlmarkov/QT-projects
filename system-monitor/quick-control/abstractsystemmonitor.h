#ifndef ABSTARCTSYSTEMMONITOR_H
#define ABSTARCTSYSTEMMONITOR_H

const int Kib = 1024;
const int Mib = Kib * 1024;
const int Gib = Mib * 1024;

class AbstractSystemMonitor
{
    public:
        AbstractSystemMonitor() {}
        virtual ~AbstractSystemMonitor() {}

        virtual void hwInfoGet(void) = 0;
        virtual void hwInfoShow(void) = 0;
        virtual void hwUsageGather(bool activate) = 0;
        virtual void hwUsageShow(void) = 0;
};

#endif // ABSTARCTSYSTEMMONITOR_H
