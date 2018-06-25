import QtQuick 2.4
import QtQuick.Controls 2.3

Item {
    width: 640
    height: 480
    property alias listView: listView
    property alias buttonUpdate: buttonUpdate

    Frame {
        id: frame
        anchors.fill: parent

        Frame {
            id: frameControl
            height: 54
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: parent.top

            Button {
                id: buttonUpdate
                y: -5
                width: 84
                height: 40
                text: qsTr("Update")
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Frame {
            id: frameInfo
            anchors.top: parent.top
            anchors.topMargin: 60
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.left: parent.left

            ListView {
                id: listView
                anchors.fill: parent
            }
        }
    }
}
