import QtQuick 2.9
import QtCharts 2.2
import QtQuick.Controls 2.2

import monitor.net 1.0

Item {
    NetMonitor { id: netMonitor }

    property int tick: 1
    property int devices: netMonitor.getDevices();

    Timer {
        interval: 1000;
        running: true;
        repeat: true
        onTriggered: {
            netMonitor.getNetUsage()

            for (var i = 0; i < devices; i++)
            {
                chartViewRx.series(i).append(tick, netMonitor.getDeviceRxBandwidth(i));
                chartViewTx.series(i).append(tick, netMonitor.getDeviceTxBandwidth(i));

                axisXRx.min = chartViewRx.series(i).at(0).x;
                axisXRx.max = chartViewRx.series(i).at(chartViewRx.series(i).count-1).x;

                if (axisYRx.max < chartViewRx.series(i).at(chartViewRx.series(i).count - 1).y)
                {
                    axisYRx.max = chartViewRx.series(i).at(chartViewRx.series(i).count - 1).y;
                }

                axisXTx.min = chartViewTx.series(i).at(0).x;
                axisXTx.max = chartViewTx.series(i).at(chartViewTx.series(i).count-1).x;

                if (axisYTx.max < chartViewTx.series(i).at(chartViewTx.series(i).count - 1).y)
                {
                    axisYTx.max = chartViewTx.series(i).at(chartViewTx.series(i).count - 1).y;
                }

                if (tick > 100)
                {
                    chartViewRx.series(i).remove(0);
                    chartViewTx.series(i).remove(0);
                }
            }
            tick = tick + 1;
        }
    }

    ChartView {
        id: chartViewRx
        title: "Download Graph"
        width: parent.width
        height: parent.height / 2
        anchors.top: parent.top
        animationOptions: ChartView.NoAnimation
        antialiasing: true

        ValueAxis { id: axisYRx; min: 0; max: 1000; visible: true; titleText: "Kib/sec"; }
        ValueAxis { id: axisXRx; min: 0; max: 100; visible: false; }
    }

    ChartView {
        id: chartViewTx
        title: "Upload Graph"
        width: parent.width
        height: parent.height / 2
        anchors.bottom: parent.bottom
        animationOptions: ChartView.NoAnimation
        antialiasing: true

        ValueAxis { id: axisYTx; min: 0; max: 1000; visible: true; titleText: "Kib/sec"; }
        ValueAxis { id: axisXTx; min: 0; max: 100;  visible: false; }
    }

    Component.onCompleted: {
        for (var i = 0; i < devices; i++)
        {
            chartViewRx.createSeries(LineSeries, netMonitor.getDeviceName(i), axisXRx, axisYRx);
            chartViewRx.series(i).append(tick, netMonitor.getDeviceRxBandwidth(i));

            chartViewTx.createSeries(LineSeries, netMonitor.getDeviceName(i), axisXTx, axisYTx);
            chartViewTx.series(i).append(tick, netMonitor.getDeviceTxBandwidth(i));
        }
        tick = tick + 1;
    }
}
