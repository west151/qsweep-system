import QtQuick 2.4
import surfacespectr 1.0

SpectrSurfaceForm {
    id: idSpectrSurfaceForm

    in_freq_min.text: userInterface.frequencyMin
    in_freq_max.text: userInterface.frequencyMax

    // !!!!!!!!!!!!!!!!!!!!!!!!!
    Loader {
        id: idLoader
        onLoaded: {
            console.log("Spectr params loaded ...");
            userInterface.on_read_params_spectr()
        }
    }

    Connections {
        target: userInterface
        onSendLnaGainChanged:{
            console.log("userInterface.lnaGain = ", userInterface.lnaGain);

            for(var i = 0; i < lnaGainModel.count; ++i)
                if(lnaGainModel.get(i).value === userInterface.lnaGain)
                    cbx_lna_gain.currentIndex = i
        }

        onSendVgaGainChanged:{
            console.log("userInterface.vgaGain = ", userInterface.vgaGain);

            for(var i = 0; i < vgaGainModel.count; ++i)
                if(vgaGainModel.get(i).value === userInterface.vgaGain)
                    cbx_vga_gain.currentIndex = i
        }

        onSendFFTBinWidthChanged:{
            console.log("userInterface.fftBinWidth = ", userInterface.fftBinWidth);

            for(var i = 0; i < fftSizeModel.count; ++i)
                if(fftSizeModel.get(i).value === userInterface.fftBinWidth)
                    cbx_fft_size.currentIndex = i
        }
    }

    button_params.onClicked: {
        idLoader.setSource("DialogParamsSpectr.qml")
    }

    button_sdr_info.onClicked: {
        idLoader.setSource("DialogSDRInfo.qml")
    }

    button_freq_info.onClicked: {
        idLoader.setSource("DialogFreqInfo.qml")
    }

    button_message_log.onClicked: {
        idLoader.setSource("DialogMessageLog.qml")
    }

    check_box_max_spectr.onClicked: {
        userInterface.on_spectr_max_calc(check_box_max_spectr.checked)
    }

    check_box_db_write.onClicked: {
        userInterface.on_spectr_db_write(check_box_db_write.checked)
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
        textRole: "key"
        model: ListModel {
            id: fftSizeModel
            ListElement { key: "500000 Hz"; value: 500000 }
            ListElement { key: "250000 Hz"; value: 250000 }
            ListElement { key: "125000 Hz"; value: 125000 }
            ListElement { key: "100000 Hz"; value: 100000 }
            ListElement { key: "50000 Hz"; value: 50000 }
            ListElement { key: "25000 Hz"; value: 25000 }
        }

        onActivated: {
            console.log("cbx_fft_size index:", cbx_fft_size.currentText, fftSizeModel.get(cbx_fft_size.currentIndex).value)
            userInterface.fftBinWidth = fftSizeModel.get(cbx_fft_size.currentIndex).value
        }
    }

    // RX VGA (baseband) gain, 0-62dB, 2dB steps
    cbx_vga_gain {
        currentIndex: 10
        textRole: "key"
        inputMethodHints: Qt.ImhDigitsOnly
        model: ListModel {
            id: vgaGainModel
            Component.onCompleted: {
                for (var i = 0; i <= 62; i=i+2) {
                    vgaGainModel.append({"key":i+" dB", "value":i})
                }
            }
        }

        onActivated: {
            console.log("cbx_vga_gain index:", cbx_vga_gain.currentText, vgaGainModel.get(cbx_vga_gain.currentIndex).value)
            userInterface.vgaGain = vgaGainModel.get(cbx_vga_gain.currentIndex).value
        }
    }

    // RX LNA (IF) gain, 0-40dB, 8dB steps
    cbx_lna_gain{
        currentIndex: 4
        textRole: "key"
        model: ListModel {
            id: lnaGainModel
            ListElement { key: "0 dB"; value: 0 }
            ListElement { key: "8 dB"; value: 8 }
            ListElement { key: "16 dB"; value: 16 }
            ListElement { key: "24 dB"; value: 24 }
            ListElement { key: "32 dB"; value: 32 }
            ListElement { key: "40 dB"; value: 40 }
        }
        onActivated: {
            console.log("cbx_lna_gain index:", cbx_lna_gain.currentText, lnaGainModel.get(cbx_lna_gain.currentIndex).value)
            userInterface.lnaGain = lnaGainModel.get(cbx_lna_gain.currentIndex).value // cbx_lna_gain.currentText
        }
    }

    // start spectr
    button_start.onClicked: {
        userInterface.frequencyMin = in_freq_min.text
        userInterface.frequencyMax = in_freq_max.text
        userInterface.lnaGain = lnaGainModel.get(cbx_lna_gain.currentIndex).value
        userInterface.vgaGain = vgaGainModel.get(cbx_vga_gain.currentIndex).value
        userInterface.fftBinWidth = fftSizeModel.get(cbx_fft_size.currentIndex).value
        userInterface.oneShot = false
        userInterface.onRequestSweepSpectr(true)
    }
    // stop spectr (stream)
    button_stop.onClicked: {
        userInterface.onRequestSweepSpectr(false)
    }

    // save params
    button_save.onClicked: {
        userInterface.frequencyMin = in_freq_min.text
        userInterface.frequencyMax = in_freq_max.text
        userInterface.lnaGain = lnaGainModel.get(cbx_lna_gain.currentIndex).value
        userInterface.vgaGain = vgaGainModel.get(cbx_vga_gain.currentIndex).value
        userInterface.fftBinWidth = fftSizeModel.get(cbx_fft_size.currentIndex).value
        userInterface.on_save_params_spectr("test")
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
