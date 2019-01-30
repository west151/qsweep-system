import QtQuick 2.4
import surfacespectr 1.0

SpectrSurfaceForm {
    id: idSpectrSurfaceForm

    Loader {
        id: idLoader
        onLoaded: {
            console.log("Spectr params loaded ...");
            userInterface.on_read_params_spectr()
        }
    }

    button_params.onClicked: {
        idLoader.setSource("DialogParamsSpectr.qml")
    }

    check_box_max_spectr.onClicked: {
        userInterface.on_spectr_max_calc(check_box_max_spectr.checked)
    }

    in_freq_min{
        //validator : RegExpValidator { regExp : /[0-9]+\.[0-9]+/ }
        validator : IntValidator{bottom: 30; top: 6000;}
        inputMethodHints: Qt.ImhDigitsOnly
    }

    in_freq_max{
        validator : IntValidator{bottom: 30; top: 6000;}
        inputMethodHints: Qt.ImhDigitsOnly
    }

    SurfaceSpectr {
        id: idSpectrItem
        objectName: "objSurfaceSpectr"
        anchors.fill: item_spectr_surface
    }

    cbx_fft_size {
        currentIndex: 2
        model: ListModel {
            id: fftSizeModel
            ListElement { text: "500000" }
            ListElement { text: "250000" }
            ListElement { text: "125000" }
            ListElement { text: "100000" }
            ListElement { text: "50000" }
            ListElement { text: "25000" }
        }

        onActivated: {
            console.log("cbx_fft_size index:", cbx_fft_size.currentText)
            userInterface.fftBinWidth = cbx_fft_size.currentText
        }
    }

    // RX VGA (baseband) gain, 0-62dB, 2dB steps
    cbx_vga_gain {
        currentIndex: 10
        inputMethodHints: Qt.ImhDigitsOnly
        model: ListModel {
            id: vgaGainModel
            Component.onCompleted: {
                for (var i = 0; i <= 62; i=i+2) {
                    vgaGainModel.append({"text":i})
                }
            }
        }

        onActivated: {
            console.log("cbx_vga_gain index:", cbx_vga_gain.currentText)
            userInterface.vgaGain = cbx_vga_gain.currentText
        }
    }

    // RX LNA (IF) gain, 0-40dB, 8dB steps
    cbx_lna_gain{
        currentIndex: 4
        model: ListModel {
            id: lnaGainModel
            ListElement { text: "0" }
            ListElement { text: "8" }
            ListElement { text: "16" }
            ListElement { text: "24" }
            ListElement { text: "32" }
            ListElement { text: "40" }
        }
        onActivated: {
            console.log("cbx_lna_gain index:", cbx_lna_gain.currentText)
            userInterface.lnaGain = cbx_lna_gain.currentText
        }
    }

    // start spectr
    button_start.onClicked: {
        userInterface.frequencyMin = in_freq_min.text
        userInterface.frequencyMax = in_freq_max.text
        userInterface.lnaGain = cbx_lna_gain.currentText
        userInterface.vgaGain = cbx_vga_gain.currentText
        userInterface.fftBinWidth = cbx_fft_size.currentText
        userInterface.oneShot = false
        userInterface.onRequestSweepSpectr(true)
    }
    // stop spectr (stream)
    button_stop.onClicked: {
        userInterface.onRequestSweepSpectr(false)
    }
}






//Connections {
//    target: stateSweepClient

//    onSendStateConnectToBroker: {

//        if(stateSweepClient.stateConnectToBroker){
//            textInputFreqMin.enabled = true
//            textInputFreqMax.enabled = true
//            cbxLNAGain.enabled = true
//            cbxVGAGain.enabled = true
//            textInputFFTBinWidth.enabled = true
//            switchOneShot.enabled = true
//            btnStart.enabled = true
//            btnStop.enabled = false
//            btnClearMaxSpectr.enabled = true
//        }else{
//            textInputFreqMin.enabled = false
//            textInputFreqMax.enabled = false
//            cbxLNAGain.enabled = false
//            cbxVGAGain.enabled = false
//            textInputFFTBinWidth.enabled = false
//            switchOneShot.enabled = false
//            btnStart.enabled = false
//            btnStop.enabled = false
//            btnClearMaxSpectr.enabled = false
//        }
//    }
//}
