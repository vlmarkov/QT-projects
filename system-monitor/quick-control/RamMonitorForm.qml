import QtQuick 2.9
import QtCharts 2.2
import QtQuick.Controls 2.2

import monitor.ram 1.0

Item {
    RamMonitor { id: ramMonitor }

    property real usedRam: ramMonitor.getUsedRam();
    property real freeRam: ramMonitor.getFreeRam();

    Timer {
        id: timer1
        interval: 1000;
        running: true;
        repeat: true
        onTriggered: {
            ramMonitor.updateHwInfo();
            usedRam = ramMonitor.getUsedRam();
            freeRam = ramMonitor.getFreeRam();
        }
    }

    ChartView {
        title: "System Memory usage"
        anchors.fill: parent
        legend.alignment: Qt.AlignBottom
        theme: ChartView.ChartThemeLight
        antialiasing: true

        HorizontalStackedBarSeries {
            axisY: BarCategoryAxis { visible: false }
            axisX: BarCategoryAxis { visible: false }
            BarSet { label: "Use "  + usedRam.toFixed(2) + " Gib"; values: [usedRam] }
            BarSet { label: "Free " + freeRam.toFixed(2) + " Gib"; values: [freeRam] }
        }
    }
}
