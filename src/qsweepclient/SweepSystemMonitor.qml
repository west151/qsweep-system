import QtQuick 2.4

SweepSystemMonitorForm {

    textHostName {
        text: qsTr("host name: ") + systemMonitorInterface.hostName
    }

    textUptime {
        text: qsTr("uptime: ") + systemMonitorInterface.uptime
    }

    textTotalMemory {
        text: qsTr("total memory: ") + (systemMonitorInterface.totalMemory/1024).toFixed(1) + qsTr(" Мб")
    }

    textFreeMemory {
        text: qsTr("free memory: ") + (systemMonitorInterface.freeMemory/1024).toFixed(1) + qsTr(" Мб")
    }

    textBufferMemory {
        text: qsTr("buffer memory: ") + (systemMonitorInterface.bufferMemory/1024).toFixed(1) + qsTr(" Мб")
    }

    //************************************************************************************
    listViewMessageLog.model: messageLogModel
    listViewMessageLog.delegate: viewLogDelegate

    listViewMessageLog {
        onCountChanged: {
            listViewMessageLog.positionViewAtEnd()
        }
    }

    Component {
        id: viewLogDelegate
        Item {
            id: mainItem
            x: 5
            width: listViewMessageLog.width
            height: idTextInfo.height

            Row {
                id: idRow
                anchors.verticalCenter: parent.verticalCenter

                Text {
                    id: idTextInfo
                    width: mainItem.width
                    text: "[" + dt_message + "] "
                          + message_text
                    wrapMode: Text.WordWrap
                    anchors.verticalCenter: parent.verticalCenter
                    font.pointSize: 13
                }
                spacing: 10
            }
        }
    }
}
