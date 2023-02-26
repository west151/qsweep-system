import QtQuick
import QtQuick.Controls

Dialog {
    id: idDialogMessageLog
    visible: true
    title: "Message Log"
    width: 600
    height: 400
    modal: true
    closePolicy: Popup.NoAutoClose
    standardButtons: Dialog.Ok

    MessageLog{
        id: idMessageLog
        anchors.fill: parent
    }
}
