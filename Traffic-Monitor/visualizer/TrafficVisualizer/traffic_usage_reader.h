#ifndef _TRAFFIC_USAGE_READER_H_
#define _TRAFFIC_USAGE_READER_H_

#include <QObject>
#include <QVector>

#include "traffic_usage_data.h"

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>

typedef struct
{
    char ifName[32];
    long rxBytes;
    long txBytes;
} trafficRawData_t;

class TrafficUsageReader : public QObject
{
    Q_OBJECT
    public:
        explicit TrafficUsageReader(QObject* parent = 0);
        ~TrafficUsageReader();

        Q_INVOKABLE int getDevices();

        Q_INVOKABLE int getHistorySize();
        Q_INVOKABLE QStringList getHistory();

        Q_INVOKABLE double getDevicesMaxTx();
        Q_INVOKABLE double getDevicesMaxRx();

        Q_INVOKABLE QString getDevicesLabelTx();
        Q_INVOKABLE QString getDevicesLabelRx();

        Q_INVOKABLE QString getDeviceName(const int device);
        Q_INVOKABLE double getDeviceTxTotal(const int device);
        Q_INVOKABLE double getDeviceRxTotal(const int device);
        Q_INVOKABLE double getDeviceTxPerDate(const int device, const int date);
        Q_INVOKABLE double getDeviceRxPerDate(const int device, const int date);

    private:
        QStringList dates_;
        QVector<DeviceTraffic> devices_;

        void readFromFile_(QVector<DateTraffic>& v, const char* path, const QString& date);
        void setDatesHistory_(const QVector<DateTraffic>& v);
        void setDevices_(const QVector<DateTraffic>& v);
        void gatherFiles_(boost::filesystem::path dir, QStringList& list);
        QString fileNameToDate_(QString string);
};

#endif // _TRAFFIC_USAGE_READER_H_
