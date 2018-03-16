import QtQuick 2.9
import QtQuick.Controls 2.2

ApplicationWindow {
    id: window
    visible: true
    width: 640*2
    height: 480*2
    title: qsTr("System Monitor")

    header: ToolBar {
        contentHeight: toolButton.implicitHeight

        ToolButton {
            id: toolButton
            text: stackView.depth > 1 ? "\u25C0" : "\u2630"
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            onClicked: {
                if (stackView.depth > 1) {
                    stackView.pop()
                } else {
                    drawer.open()
                }
            }
        }
    }

    Drawer {
        id: drawer
        width: window.width * 0.45
        height: window.height

        Column {
            anchors.fill: parent

            ItemDelegate {
                text: qsTr("CPU")
                width: parent.width
                onClicked: {
                    stackView.push("CpuMonitorForm.qml")
                    drawer.close()
                }
            }

            ItemDelegate {
                text: qsTr("RAM")
                width: parent.width
                onClicked: {
                    stackView.push("RamMonitorForm.qml")
                    drawer.close()
                }
            }

            ItemDelegate {
                text: qsTr("HDD")
                width: parent.width
                onClicked: {
                    stackView.push("HddMonitorForm.qml")
                    drawer.close()
                }
            }

            ItemDelegate {
                text: qsTr("Network")
                width: parent.width
                onClicked: {
                    stackView.push("NetMonitorForm.qml")
                    drawer.close()
                }
            }

            ItemDelegate {
                text: qsTr("Processes")
                width: parent.width
                onClicked: {
                    stackView.push("ProcessesMonitorForm.qml")
                    drawer.close()
                }
            }
        }
    }

    StackView {
        id: stackView
        initialItem: "MainForm.qml"
        anchors.fill: parent
    }
}
