#ifndef _TRAFFIC_DAEMON_H_
#define _TRAFFIC_DAEMON_H_

#include "traffic_storage.h"
#include "traffic_monitor.h"

#include <csignal>

class TrafficDaemon
{
    public:
        TrafficDaemon(int argc, char* argv[]);
        ~TrafficDaemon();

        void startDaemon();

    private:
        static void signalTrace_(int sig, siginfo_t *si, void *ptr);
        static void signalQuit_(int sig);
        void getTraffic_();

        TrafficMonitor trafficMonitor_;
        TrafficStorage trafficStorage_;

        static bool quit_; // not so good aproach
};

#endif // _TRAFFIC_DAEMON_H_
