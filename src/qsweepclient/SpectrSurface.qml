import QtQuick 2.4
import surfacespectr 1.0

SpectrSurfaceForm {



    // property alias check_box_max_spectr: check_box_max_spectr

    check_box_max_spectr.onClicked: {
        userInterface.on_spectr_max_calc(check_box_max_spectr.checked)
        //userInterface.onRequestSweepSpectr(false)
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
        model: fftSizeModel
        currentIndex: 2
    }

    ListModel {
        id: fftSizeModel
        ListElement { text: "500000" }
        ListElement { text: "250000" }
        ListElement { text: "125000" }
        ListElement { text: "100000" }
        ListElement { text: "50000" }
        ListElement { text: "25000" }
    }

    cbx_vga_gain {
        model: vgaGainModel
        currentIndex: 10
    }

    // RX VGA (baseband) gain, 0-62dB, 2dB steps
    ListModel {
        id: vgaGainModel
        Component.onCompleted:
        {
            for (var i = 0; i <= 62; i=i+2) {
                vgaGainModel.append({"text":i})
            }
        }
    }

    cbx_lna_gain{
        model: lnaGainModel
        currentIndex: 4
    }

    // RX LNA (IF) gain, 0-40dB, 8dB steps
    ListModel {
        id: lnaGainModel
        ListElement { text: "0" }
        ListElement { text: "8" }
        ListElement { text: "16" }
        ListElement { text: "24" }
        ListElement { text: "32" }
        ListElement { text: "40" }
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
