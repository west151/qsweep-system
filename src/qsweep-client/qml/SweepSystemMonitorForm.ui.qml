import QtQuick 2.4
import QtQuick.Controls 2.3

Item {
    id: item1
    width: 800
    height: 600
    property alias textBufferMemory: textBufferMemory
    property alias textFreeMemory: textFreeMemory
    property alias textTotalMemory: textTotalMemory
    property alias textUptime: textUptime
    property alias textHostName: textHostName

    Item {
        id: itemHostInfo
        anchors.fill: parent

        Frame {
            id: frame
            anchors.fill: parent

            Column {
                x: 155
                y: -12
                anchors.top: parent.top
                anchors.topMargin: 8
                anchors.verticalCenterOffset: -183
                anchors.horizontalCenterOffset: 0
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                spacing: 15

                Text {
                    id: textHostName
                    width: 50
                    height: 29
                    text: qsTr("Text")
                    font.pixelSize: 25
                }

                Text {
                    id: textUptime
                    text: qsTr("Text")
                    font.pixelSize: 25
                }

                Text {
                    id: textTotalMemory
                    text: qsTr("Text")
                    font.pixelSize: 25
                }

                Text {
                    id: textFreeMemory
                    text: qsTr("Text")
                    font.pixelSize: 25
                }

                Text {
                    id: textBufferMemory
                    text: qsTr("Text")
                    font.pixelSize: 25
                }
            }
        }
    }
}

/*##^## Designer {
    D{i:23;anchors_y:0}D{i:24;anchors_height:228;anchors_width:338;anchors_x:0;anchors_y:40}
D{i:10;anchors_height:292;anchors_width:362;anchors_x:8;anchors_y:8}D{i:12;anchors_y:0}
D{i:13;anchors_height:207;anchors_width:338;anchors_x:0;anchors_y:39}D{i:11;anchors_height:270;anchors_width:362;anchors_x:8;anchors_y:306}
D{i:8;anchors_height:584;anchors_width:378;anchors_x:414;anchors_y:8}
}
 ##^##*/
