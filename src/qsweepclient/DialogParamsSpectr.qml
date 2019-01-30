import QtQuick 2.0
import QtQuick.Dialogs 1.3

Dialog {
    id: dialogParamsSpectr
    visible: true
    title: "Spectr params"
    width: 600
    height: 400
    standardButtons: Dialog.Ok | Dialog.Cancel

    modality : Qt.ApplicationModal

    ParamsSpectr{
        anchors.fill: parent
    }

    onAccepted: console.log("Ok clicked")
    onRejected: console.log("Cancel clicked")
}
