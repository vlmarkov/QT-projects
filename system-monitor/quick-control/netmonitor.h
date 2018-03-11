#ifndef NETMONITOR_H
#define NETMONITOR_H

#include <QObject>

#include <vector>

class NetUsage
{
    public:
        NetUsage(QString name, QString addr, long txBytes, long rxBytes,  double txBandwidth, double  rxBandwidth) :
            ifName_(name), ipAddr_(addr), txBytes_(txBytes), rxBytes_(rxBytes), txBandwidth_(txBandwidth), rxBandwidth_(rxBandwidth) {}

        QString ifName_;
        QString ipAddr_;
        long    txBytes_;
        long    rxBytes_;
        double  txBandwidth_;
        double  rxBandwidth_;
};

class NetMonitor : public QObject
{
    Q_OBJECT
    public:
        explicit NetMonitor(QObject* parent = 0);
        ~NetMonitor();

        void getNetInfo();

        Q_INVOKABLE void getNetUsage();
        Q_INVOKABLE int getDevices();
        Q_INVOKABLE QString getDeviceName(int device);
        Q_INVOKABLE double getDeviceTxBandwidth(int device);
        Q_INVOKABLE double getDeviceRxBandwidth(int device);

    private:
        std::vector<NetUsage> NetUsage_;
};

#endif // NETMONITOR_H
