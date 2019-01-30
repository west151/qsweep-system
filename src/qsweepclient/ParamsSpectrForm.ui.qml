import QtQuick 2.4
import QtQuick.Controls 2.3

Item {
    id: element
    width: 600
    height: 400
    property alias listViewParamsSpectr: listViewParamsSpectr
    property alias button_add: button_add

    Frame {
        id: frame_list_params
        anchors.right: parent.right
        anchors.rightMargin: 165
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 8

        ListView {
            id: listViewParamsSpectr
            anchors.fill: parent
        }
    }

    Frame {
        id: frame_control
        x: 441
        width: 151
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 8

        Button {
            id: button_add
            height: 40
            text: qsTr("add")
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0
        }
    }
}
