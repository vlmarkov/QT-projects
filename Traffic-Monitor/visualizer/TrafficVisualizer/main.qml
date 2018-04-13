import QtQuick 2.9
import QtCharts 2.2
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

import trafficUsageReader 1.0

Item {
    TrafficUsageReader { id: networkUsageReader }

    property real   devices: networkUsageReader.getDevices();
    property real   maxRx:   networkUsageReader.getDevicesMaxRx();
    property real   maxTx:   networkUsageReader.getDevicesMaxTx();
    property int    dates:   networkUsageReader.getHistorySize();
    property string labelRx: networkUsageReader.getDevicesLabelRx()
    property string labelTx: networkUsageReader.getDevicesLabelTx()

    width: Screen.width
    height: Screen.height

    ChartView {
        id: chartCommonUsageRx

        theme: ChartView.ChartThemeLight
        antialiasing: true
        legend.visible: false
        animationOptions: ChartView.AllAnimations

        title: "Common overview for download"

        width: parent.width / 2
        height: parent.height / 2
        anchors.left: parent.left
        anchors.top: parent.top

        PieSeries {
            id: seriesCommonRx
            holeSize: 0.5
        }
    }

    ChartView {
        id: chartCommonUsageTx

        theme: ChartView.ChartThemeLight
        antialiasing: true
        legend.visible: false
        animationOptions: ChartView.AllAnimations

        title: "Common overview for upload"

        width: parent.width / 2
        height: parent.height / 2
        anchors.right: parent.right
        anchors.top: parent.top

        PieSeries {
            id: seriesCommonTx
            holeSize: 0.5
        }
    }

    ChartView {
        id: chartDetailedUsageRx

        theme: ChartView.ChartThemeLight
        antialiasing: true
        legend.alignment: Qt.AlignRight
        animationOptions: ChartView.AllAnimations

        width: parent.width / 2
        height: parent.height / 2
        anchors.left: parent.left
        anchors.bottom: parent.bottom

        title: "Usage details for download"

        BarSeries {
            id: seriesDetailedRx

           axisY: ValueAxis {
                id: axisYRx
                titleText: labelRx
                min: 0;
                max: maxRx;
            }

            axisX: BarCategoryAxis {
                categories: networkUsageReader.getHistory()
           }
        }
    }

    ChartView {
        id: chartDetailedUsageTx

        theme: ChartView.ChartThemeLight
        antialiasing: true
        legend.alignment: Qt.AlignRight
        animationOptions: ChartView.AllAnimations

        width: parent.width / 2
        height: parent.height / 2

        anchors.right: parent.right
        anchors.bottom: parent.bottom

        title: "Usage details for upload"

        BarSeries {
            id: seriesDetailedTx

           axisY: ValueAxis {
                id: axisYTx
                titleText: labelTx
                min: 0;
                max: maxTx;
            }

            axisX: BarCategoryAxis {
                categories: networkUsageReader.getHistory()
           }
        }
    }

    Component.onCompleted: {
        var maxIdxRx = 0;
        var maxIdxTx = 0;

        for (var i = 0; i < devices; i++)
        {
            seriesCommonRx.append(networkUsageReader.getDeviceName(i) + ": "
                                  + networkUsageReader.getDeviceRxTotal(i).toFixed(2)
                                  + " " + labelRx,
                                  networkUsageReader.getDeviceRxTotal(i));

            seriesCommonRx.at(i).labelPosition = PieSlice.LabelOutside;
            seriesCommonRx.at(i).labelVisible  = true;
            seriesCommonRx.at(i).borderWidth   = 3;

            if (networkUsageReader.getDeviceRxTotal(maxIdxRx) < networkUsageReader.getDeviceRxTotal(i))
            {
                maxIdxRx = i;
            }

            seriesCommonTx.append(networkUsageReader.getDeviceName(i) + ": "
                                 + networkUsageReader.getDeviceTxTotal(i).toFixed(2)
                                 + " " + labelTx,
                                 networkUsageReader.getDeviceTxTotal(i));

            seriesCommonTx.at(i).labelPosition = PieSlice.LabelOutside;
            seriesCommonTx.at(i).labelVisible  = true;
            seriesCommonTx.at(i).borderWidth   = 3;

            if (networkUsageReader.getDeviceTxTotal(maxIdxTx) < networkUsageReader.getDeviceTxTotal(i))
            {
                maxIdxTx = i;
            }
        }

        seriesCommonRx.find(networkUsageReader.getDeviceName(maxIdxRx)
                            + ": "
                            + networkUsageReader.getDeviceRxTotal(maxIdxRx).toFixed(2)
                            + " " + labelRx).exploded = true;

        seriesCommonTx.find(networkUsageReader.getDeviceName(maxIdxTx)
                            + ": "
                            + networkUsageReader.getDeviceTxTotal(maxIdxTx).toFixed(2)
                            + " " + labelTx).exploded = true;

        for (var i = 0; i < devices; i++)
        {
            seriesDetailedRx.append(networkUsageReader.getDeviceName(i), []);
            seriesDetailedTx.append(networkUsageReader.getDeviceName(i), []);
            for (var j = 0; j < dates; j++)
            {
                seriesDetailedRx.at(i).append(networkUsageReader.getDeviceRxPerDate(i, j));
                seriesDetailedTx.at(i).append(networkUsageReader.getDeviceTxPerDate(i, j));
            }
        }
    }
}
