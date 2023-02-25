import QtQuick
import QtQuick.Controls

Dialog {
    id: idDialogSDRInfo
    visible: true
    title: "SDR Information"
    width: 600
    height: 400
    standardButtons: Dialog.Ok

//    modality : Qt.ApplicationModal

    SDRInfo{
        id: idSDRInfo
        anchors.fill: parent
    }
}
