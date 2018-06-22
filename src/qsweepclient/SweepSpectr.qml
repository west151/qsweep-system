import QtQuick 2.4

SweepSpectrForm {

    textInputFreqMin{
        //validator : RegExpValidator { regExp : /[0-9]+\.[0-9]+/ }
        validator : IntValidator{bottom: 30; top: 6000;}
    }

    textInputFreqMax{
        validator : IntValidator{bottom: 30; top: 6000;}
    }

    btnStart.onClicked: {
        userInterface.frequencyMin = textInputFreqMin.text
        userInterface.frequencyMax = textInputFreqMax.text
        userInterface.onRequestSweepSpectr()
    }
}
