import QtQuick
import QtQuick.Controls

Dialog {
    id: dialogParamsSpectr
    visible: true
    title: "Spectr params"
    width: 600
    height: 400
    standardButtons: Dialog.Ok | Dialog.Cancel

//    modality : Qt.ApplicationModal

    ParamsSpectr{
        id: idParamsSpectr
        anchors.fill: parent
    }

    onAccepted: {
        paramsSpectrModel.get_params_by_index(idParamsSpectr.cIndex)
        console.log(idParamsSpectr.cIndex)
        console.log("Ok clicked")
    }

    onRejected: console.log("Cancel clicked")
}
