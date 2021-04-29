import QtQuick 2.4
import QtQuick.Controls 2.3

Item {
    width: 600
    height: 400
    property alias list_view_message_log: list_view_message_log

    Frame {
        id: frame
        anchors.fill: parent

        ListView {
            id: list_view_message_log
            anchors.fill: parent
        }
    }
}
