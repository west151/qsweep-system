import QtQuick 2.4
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.0

Item {
    id: item1
    width: 640
    height: 480
    property alias labelPingReceivedCount: labelPingReceivedCount
    property alias labelPortBroker: labelPortBroker
    property alias labelHostBroker: labelHostBroker

    Frame {
        id: frame
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 8

        Grid {
            id: grid
            width: 258
            height: 97
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
            spacing: 11
            columns: 2

            Label {
                id: labelHost
                text: qsTr("Host broker:")
                font.pointSize: 14
            }

            Label {
                id: labelHostBroker
                text: qsTr("host")
                font.pointSize: 14
            }

            Label {
                id: labelPort
                text: qsTr("Port broker:")
                font.pointSize: 14
            }

            Label {
                id: labelPortBroker
                text: qsTr("port")
                font.pointSize: 14
            }

            Label {
                id: labelPingCount
                text: qsTr("Ping Received:")
                font.pointSize: 14
            }

            Label {
                id: labelPingReceivedCount
                text: qsTr("count")
                font.pointSize: 14
            }
        }
    }
}
