import QtQuick 2.4

Item {
    width: 600
    height: 40
    property alias labelBroker: labelBroker
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

        Text {
            id: labelBroker
            x: 13
            y: 8
            width: 174
            height: 23
            color: "#ffffff"
            text: qsTr("127.0.0.1:1883[0]")
            font.pixelSize: 20
        }
    }
}
