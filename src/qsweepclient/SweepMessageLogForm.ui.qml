import QtQuick 2.4
import QtQuick.Controls 2.3

Item {
    width: 640
    height: 480
    property alias listViewMessageLog: listViewMessageLog

    Frame {
        id: frame
        anchors.fill: parent

        Frame {
            id: frameMessageLog
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0

            ListView {
                id: listViewMessageLog
                anchors.bottomMargin: 15
                anchors.fill: parent
                delegate: Item {
                    x: 5
                    width: 80
                    height: 40
                    Row {
                        id: row1
                        spacing: 10
                        Rectangle {
                            width: 40
                            height: 40
                            color: colorCode
                        }

                        Text {
                            text: name
                            anchors.verticalCenter: parent.verticalCenter
                            font.bold: true
                        }
                    }
                }
            }
        }
    }
}
