#include "CpuMonitor.hpp"

int main(int argc, char const *argv[])
{
    AbstractSystemMonitor* cpuMonitor = new CpuMonitor;

    cpuMonitor->hwInfoGet();
    cpuMonitor->hwInfoShow();
    //cpuMonitor->hwUsageGather(true);
    cpuMonitor->hwUsageShow();

    delete cpuMonitor;

    return 0;
}