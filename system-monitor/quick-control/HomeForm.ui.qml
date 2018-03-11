import QtQuick 2.9
import QtCharts 2.2
import QtQuick.Controls 2.2

import monitor.cpu 1.0

Page {
    CpuMonitor {
        id: cpuMonitor
    }

     property string someString: cpuMonitor.getArchStr();

    Column {
        spacing: 40
        width: parent.width

        Label {
            id: arch
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: someString;
            //value: cpuMonitor.getArchStr();
        }

        Label {
            id: vendor
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: cpuMonitor.getVendorStr();
        }

        Label {
            id: model
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: cpuMonitor.getModelStr();
        }

        Label {
            id: brand
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: cpuMonitor.getBrandStr();
        }

        Label {
            id: hwCores
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: cpuMonitor.getHwCoresStr();
        }

        Label {
            id: logCores
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: cpuMonitor.getLogCoresStr();
        }

        Label {
            id: l1
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: cpuMonitor.getL1Str();
        }

        Label {
            id: l2
            width: parent.width
            wrapMode: Label.Wrap
            horizontalAlignment: Qt.AlignHCenter
            text: cpuMonitor.getL2Str();
        }
    }
}
