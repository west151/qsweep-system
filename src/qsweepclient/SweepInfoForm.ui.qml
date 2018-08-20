import QtQuick 2.4
import QtQuick.Controls 2.3

Item {
    width: 640
    height: 480
    property alias listView: listView

    Frame {
        id: frame
        anchors.fill: parent

        Frame {
            id: frameInfo
            anchors.top: parent.top
            anchors.topMargin: 0
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
