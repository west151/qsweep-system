import QtQuick 2.4

SweepMessageLogForm {

    listViewMessageLog.model: messageLogModel
    listViewMessageLog.delegate: viewLogDelegate

    Component {
        id: viewLogDelegate
        Item {
            id: mainItem
            x: 5
            width: listViewMessageLog.width
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
