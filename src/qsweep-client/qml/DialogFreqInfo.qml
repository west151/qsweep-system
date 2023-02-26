import QtQuick
import QtQuick.Controls

Dialog {
    id: idDialogFreqInfo
    visible: true
    title: "Frequency Information"
    width: 600
    height: 400
    modal: true
    closePolicy: Popup.NoAutoClose
    standardButtons: Dialog.Ok

    FreqInfo{
        id: idFreqInfo
        anchors.fill: parent
    }
}
