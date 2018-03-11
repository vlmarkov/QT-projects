import QtQuick 2.9
import QtCharts 2.2
import QtQuick.Controls 2.2

import monitor.cpu 1.0
import monitor.hdd 1.0
import monitor.os 1.0

Item {
    CpuMonitor { id: cpuMonitor }
    HddMonitor { id: hddMonitor }
    OsMonitor  { id: osMonitor }

    Column {
        spacing: 10.0
        padding: 20.0
        width: parent.width

        Label { font.bold: true; text: "OS Name: "        + osMonitor.getOsNameStr(); }
        Label { font.bold: true; text: "Hostname: "       + osMonitor.getHostNameStr(); }
        Label { font.bold: true; text: "Username: "       + osMonitor.getUserNameStr(); }
        Label { font.bold: true; text: "Uptime: "         + osMonitor.getUpTimeStr(); }
        Label { font.bold: true; text: "Processor: "      + osMonitor.getCpuNameStr(); }
        Label { font.bold: true; text: "System memory: "  + osMonitor.getTotalRamStr(); }
        Label { font.bold: true; text: "Graphic Card: "   + "N/A"; }
        Label { font.bold: true; text: "Graphic Memory: " + "N/A"; }
        Label { font.bold: true; text: "Hard Drive: "     + "N/A"; }
        Label { font.bold: true; text: "Storage Space: "  + hddMonitor.getSpaceStr() }
    }
}
