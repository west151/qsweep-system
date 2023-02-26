import QtQuick
import QtQuick.Controls

Dialog {
    id: idDialogSDRInfo
    visible: true
    title: "SDR Information"
    width: 600
    height: 400
    modal: true
    closePolicy: Popup.NoAutoClose
    standardButtons: Dialog.Ok

    SDRInfo{
        id: idSDRInfo
        anchors.fill: parent
    }
}
