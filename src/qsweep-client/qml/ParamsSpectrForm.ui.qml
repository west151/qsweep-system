import QtQuick
import QtQuick.Controls

Item {
    id: element
    width: 600
    height: 400
    property alias listViewParamsSpectr: listViewParamsSpectr

    Frame {
        id: frame_list_params
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 8

        ListView {
            id: listViewParamsSpectr
            spacing: 5
            anchors.fill: parent
        }
    }
}
