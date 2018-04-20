#include <QString>

#include "traffic_daemon.h"

#include "syslog.h"

int main(int argc, char *argv[])
{
    TrafficDaemon trafficDaemon(argc, argv);

    try
    {
        trafficDaemon.startDaemon();
    }
    catch (QString err)
    {
        syslog(LOG_ERR, "%s", err.toLatin1().data());
        return -1;
    }

    return 0;
}
