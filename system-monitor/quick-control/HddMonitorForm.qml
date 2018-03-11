import QtQuick 2.9
import QtCharts 2.2
import QtQuick.Controls 2.2

import monitor.hdd 1.0

Item {
    HddMonitor {
        id: hddMonitor
    }

    ChartView {
        id: chart
        anchors.fill: parent
        title: "Storage space " + hddMonitor.getSpaceStr();
        //legend.alignment: Qt.AlignBottom
        legend.visible: false
        theme: ChartView.ChartThemeLight
        animationOptions: ChartView.SeriesAnimations
        antialiasing: true

        PieSeries {
            id: pieSeries
            PieSlice {
                label: hddMonitor.getUseSpaceStr();
                exploded: true;
                labelVisible: true;
                labelPosition: PieSlice.LabelOutside;
                value: hddMonitor.getUseSpace();
            }

            PieSlice {
                label: hddMonitor.getFreeSpaceStr();
                labelVisible: true;
                labelPosition: PieSlice.LabelOutside;
                value: hddMonitor.getFreeSpace();
            }
        }
    }
}
