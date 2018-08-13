import QtQuick 2.4

Item {
    width: 800
    height: 600
    property alias textBufferMemory: textBufferMemory
    property alias textFreeMemory: textFreeMemory
    property alias textTotalMemory: textTotalMemory
    property alias textUptime: textUptime
    property alias textHostName: textHostName

    Column {
        x: 8
        y: 8
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
