import QtQuick 2.4
import QtQuick.Controls 2.3

Item {
    width: 400
    height: 400

    Page {
        id: page
        anchors.fill: parent

        Text {
            id: text1
            x: 162
            y: 178
            text: qsTr("Text Sweep")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 12
        }
    }
}
