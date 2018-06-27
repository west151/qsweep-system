import QtQuick 2.4

SweepSpectrForm {

    textInputFreqMin{
        //validator : RegExpValidator { regExp : /[0-9]+\.[0-9]+/ }
        validator : IntValidator{bottom: 30; top: 6000;}
    }

    textInputFreqMax{
        validator : IntValidator{bottom: 30; top: 6000;}
    }

    textInputLNAGain {
        // RX LNA (IF) gain, 0-40dB, 8dB steps
        validator : IntValidator{bottom: 0; top: 40;}
    }

    textInputVGAGain {
        // RX VGA (baseband) gain, 0-62dB, 2dB steps
        validator : IntValidator{bottom: 0; top: 62;}
    }

    switchOneShot {
        enabled: false
    }

    btnStart.onClicked: {
        userInterface.frequencyMin = textInputFreqMin.text
        userInterface.frequencyMax = textInputFreqMax.text
        userInterface.lnaGain = textInputLNAGain.text
        userInterface.vgaGain = textInputVGAGain.text
        userInterface.fftBinWidth = textInputFFTBinWidth.text
        userInterface.oneShot = switchOneShot.checkable
        userInterface.onRequestSweepSpectr()
    }
}
