import QtQuick 2.9
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.2

import monitor.proc 1.0

Item {
    ProcMonitor { id: procMonitor }

    property int processes: procMonitor.getProccessNumbers();

    ListModel { id: model }

    ListView {
        id: list
        anchors.fill: parent
        model: model
        spacing: 5

        delegate: Component {
            Item {
                width: parent.width
                height: 60
                Column {
                    Text { text: "<b>PID :</b>" + pid }
                    Text { text: "<b>Process :</b>" + name }
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: list.currentIndex = index
                }
            }
        }
        highlight: Rectangle { color: 'grey' }
        focus: true
        //onCurrentItemChanged: console.log(model.get(list.currentIndex).pid + ' selected')
    }

    ToolButton {
        id: button1
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        text: qsTr("Kill process")
        onClicked: procMonitor.killProcces(model.get(list.currentIndex).pid)
    }

    Component.onCompleted: {
        for (var i = 0; i < processes; i++)
        {
            model.append({ "pid":  procMonitor.getProccessPid(i),
                           "name": procMonitor.getProccessName(i)});
        }
    }


}
