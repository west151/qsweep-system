import QtQuick

MessageLogForm {

    list_view_message_log {
        clip: true
        focus: true
        model: messageLogModel
        delegate: viewLogDelegate

        onCountChanged: {
            list_view_message_log.positionViewAtEnd()
        }
    }

    Component {
        id: viewLogDelegate
        Item {
            id: mainItem
            x: 5
            width: list_view_message_log.width
            height: idTextInfo.height

            Row {
                id: idRow
                anchors.verticalCenter: parent.verticalCenter

                Text {
                    id: idTextInfo
                    width: mainItem.width
                    text: "[" + dt_message + "] "
                          + message_text
                    wrapMode: Text.WordWrap
                    anchors.verticalCenter: parent.verticalCenter
                    font.pointSize: 13
                }
                spacing: 10
            }
        }
    }
}
