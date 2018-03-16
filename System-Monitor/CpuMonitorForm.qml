import QtQuick 2.9
import QtCharts 2.2
import QtQuick.Controls 2.2

import monitor.cpu 1.0

Item {
    CpuMonitor { id: cpuMonitor }

    property int tick: 1
    property int cores: cpuMonitor.getCores();

    Timer {
        interval: 1000;
        running: true;
        repeat: true
        onTriggered: {
            cpuMonitor.getCpuUsage()

            for (var i = 0; i < cores; i++)
            {
                chartCpuUsage.series(i).append(tick, cpuMonitor.getCoreUsage(i));

                axisX.min = chartCpuUsage.series(i).at(0).x;
                axisX.max = chartCpuUsage.series(i).at(chartCpuUsage.series(i).count - 1).x;

                if (tick > 100)
                {
                    chartCpuUsage.series(i).remove(0);
                }
            }
            tick = tick + 1;
        }
    }

    ChartView {
        id: chartCpuUsage
        title: "Cpu Usage Graph"
        width: parent.width
        height: parent.height
        anchors.bottom: parent.bottom
        animationOptions: ChartView.NoAnimation
        antialiasing: true

        ValueAxis { id: axisY;  min: 0; max: 100; visible: true; titleText: "Usage %"; }
        ValueAxis { id: axisX;  min: 0; max: 100; visible: false; }
    }

    Component.onCompleted: {
        for (var i = 0; i < cores; i++)
        {
            chartCpuUsage.createSeries(LineSeries, cpuMonitor.getCoreName(i), axisX, axisY);
        }
        tick = tick + 1;
    }
}
