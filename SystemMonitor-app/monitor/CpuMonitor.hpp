#ifndef _CPU_MONITOR_HPP_
#define _CPU_MONITOR_HPP_

#include "AbstractSystemMonitor.hpp"

#include <string>

class CpuMonitor : public AbstractSystemMonitor
{
    private:
        std::string arch_;
        std::string bit_;
        std::string byte_;
        std::string cores_;
        std::string threads_;
        std::string vendor_;
        std::string model_;
        std::string cpu_;
        std::string l1d_;
        std::string l1i_;
        std::string l2_;
        std::string l3_;
        std::string flags_;

        void hwInfoAsign(std::string line, int lnNum);

    public:
        CpuMonitor();
        ~CpuMonitor();

        void hwInfoGet();
        void hwInfoShow();
        void hwUsageGather(bool activate);
        void hwUsageShow();
};


#endif /* _CPU_MONITOR_HPP_ */
