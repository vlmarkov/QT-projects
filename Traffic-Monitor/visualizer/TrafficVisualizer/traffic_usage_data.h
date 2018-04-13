#ifndef _TRAFFIC_USAGE_DATA_H_
#define _TRAFFIC_USAGE_DATA_H_

#include <QVector>
#include <QString>

const QString kibStr("Kib");
const QString mibStr("Mib");
const QString gibStr("Gib");

constexpr int kibInt = (1024);
constexpr int mibInt = (kibInt * 1024);
constexpr int gibInt = (mibInt * 1024);

class DateTraffic
{
    public:
        DateTraffic();
        DateTraffic(const QString& date,
                    const QString& device,
                    const long     rxBytes,
                    const long     txBytes);
        ~DateTraffic();

        QString _date;
        QString _device;
        long    _rxBytes;
        long    _txBytes;
};

class DeviceTraffic
{
    public:
        DeviceTraffic();
        DeviceTraffic(const QString& name);
        ~DeviceTraffic();

        void addDateTraffic(DateTraffic& dateTraffic);
        void convertUsageTo(const QString& strUsage,
                            const int&     intUsage);

        QString _device;
        QString _dataTypeRx;
        QString _dateTypeTx;
        double  _rxTotal;
        double  _txTotal;
        QVector<DateTraffic> _history;
};

#endif // _TRAFFIC_USAGE_DATA_H_
