import QtQuick 2.4

Item {
    width: 800
    height: 40
    property alias mainRectangle: mainRectangle
    property alias textFrame: textFrame
    property alias textTime: textTime

    Rectangle {
        id: mainRectangle
        color: "#000000"
        anchors.fill: parent

        Text {
            id: textTime
            x: 713
            width: 79
            height: 24
            color: "#ffffff"
            text: "00:00:00"
            anchors.verticalCenterOffset: 0
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 8
            font.bold: false
            font.pixelSize: 20
        }

        Text {
            id: textFrame
            x: 388
            y: 13
            color: "#ffffff"
            text: qsTr("")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 20
        }
    }
}
