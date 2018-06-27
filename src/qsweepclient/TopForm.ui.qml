import QtQuick 2.4

Item {
    width: 600
    height: 40
    property alias textTime: textTime

    Rectangle {
        id: rectangle
        color: "#000000"
        anchors.fill: parent

        Text {
            id: textTime
            x: 540
            width: 51
            height: 24
            color: "#ffffff"
            text: "00:00"
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 15
            font.bold: false
            font.pixelSize: 20
        }
    }
}
