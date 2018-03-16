#ifndef NETMONITOR_H
#define NETMONITOR_H

#include <QObject>
#include <QVector>

class NetUsage
{
    public:
        NetUsage();
        NetUsage(const QString _name,
                 const QString _addr,
                 const long    _txBytes,
                 const long    _rxBytes,
                 const double  _txBandwidth,
                 const double  _rxBandwidth);

        QString ifName;
        QString ipAddr;
        long    txBytes;
        long    rxBytes;
        double  txBandwidth;
        double  rxBandwidth;
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
        Q_INVOKABLE QString getDeviceName(const int i);
        Q_INVOKABLE double getDeviceTxBandwidth(const int i);
        Q_INVOKABLE double getDeviceRxBandwidth(const int i);

    private:
        QVector<NetUsage> NetUsage_;
};

#endif // NETMONITOR_H
