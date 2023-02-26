import QtQuick
import QtQuick.Controls

Item {
    width: 600
    height: 400
    property alias list_view_sdr_info: list_view_sdr_info

    Frame {
        id: main_frame
        anchors.fill: parent

        ListView {
            id: list_view_sdr_info
            anchors.fill: parent
        }
    }
}
