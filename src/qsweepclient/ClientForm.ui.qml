import QtQuick 2.4
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.0

Item {
    width: 640
    height: 480
    property alias buttonDisconnect: buttonDisconnect
    property alias buttonSendData: buttonSendData
    property alias buttonConnect: buttonConnect
    property alias textEditPort: textEditPort
    property alias textEditAddress: textEditAddress

    Page {
        id: page
        anchors.fill: parent

        Text {
            id: textAddress
            x: 21
            y: 20
            text: qsTr("ip:")
            font.bold: true
            font.pixelSize: 12
        }

        TextEdit {
            id: textEditAddress
            x: 40
            y: 20
            width: 53
            height: 13
            text: qsTr("127.0.0.1")
            horizontalAlignment: Text.AlignLeft
            font.pixelSize: 12
        }

        TextEdit {
            id: textEditPort
            x: 131
            y: 20
            height: 13
            text: qsTr("1883")
            Layout.preferredHeight: 20
            Layout.preferredWidth: 80
            font.pixelSize: 12
        }

        Text {
            id: textPort
            x: 99
            y: 20
            text: qsTr("port:")
            font.bold: true
            font.pixelSize: 12
        }

        Button {
            id: buttonConnect
            x: 175
            y: 14
            width: 93
            height: 26
            text: qsTr("Connect")
        }

        Button {
            id: buttonSendData
            x: 288
            y: 14
            width: 93
            height: 26
            text: qsTr("Send data")
        }

        Button {
            id: buttonDisconnect
            x: 175
            y: 56
            width: 93
            height: 26
            text: qsTr("Disconnect")
        }
    }
}
