import QtQuick
import QtQuick.Controls

Dialog {
    id: idDialogMessageLog
    visible: true
    title: "Message Log"
    width: 600
    height: 400
    standardButtons: Dialog.Ok

//    modality : Qt.ApplicationModal

    MessageLog{
        id: idMessageLog
        anchors.fill: parent
    }
}
