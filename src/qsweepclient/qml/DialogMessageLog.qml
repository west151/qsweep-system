import QtQuick 2.0
import QtQuick.Dialogs 1.3

Dialog {
    id: idDialogMessageLog
    visible: true
    title: "Message Log"
    width: 600
    height: 400
    standardButtons: Dialog.Ok

    modality : Qt.ApplicationModal

    MessageLog{
        id: idMessageLog
        anchors.fill: parent
    }
}
