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
    listViewSDRInfo.model: hackrfInfoModel
    listViewSDRInfo.delegate: viewInfoDelegate

    Component {
        id: viewInfoDelegate
        Item {
            id: mainItem
            x: 5
            width: listViewSDRInfo.width
            height: idTextInfo.height

            Row {
                id: idRow
                anchors.verticalCenter: parent.verticalCenter

                Text {
                    id: idTextInfo
                    width: mainItem.width
                    text: "Serial Numbers: " + info_serial_numbers + "\n"
                          + "Board ID: " + info_board_id + "\n"
                          + "Firmware Version: " + info_firmware_version + "\n"
                          + "Part ID Number: " + info_part_id_number + "\n"
                          + "Libhackrf Version: " + info_hackrf_version
                    wrapMode: Text.WordWrap
                    anchors.verticalCenter: parent.verticalCenter
                    font.pointSize: 13
                }
                spacing: 10
            }
        }
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
