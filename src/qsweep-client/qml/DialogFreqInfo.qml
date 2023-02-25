import QtQuick 2.0
import QtQuick.Dialogs 1.3

Dialog {
    id: idDialogFreqInfo
    visible: true
    title: "Frequency Information"
    width: 600
    height: 400
    standardButtons: Dialog.Ok

    modality : Qt.ApplicationModal

    FreqInfo{
        id: idFreqInfo
        anchors.fill: parent
    }
}
