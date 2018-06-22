import QtQuick 2.4

SweepSpectrForm {

    btnStart.onClicked: {
        userInterface.frequencyMin = textInputFreqMin.text
        userInterface.frequencyMax = textInputFreqMax.text

        userInterface.onRequestSweepSpectr()
    }


//    buttonDisconnect.onClicked: {
//        userInterface.onDisconnectFromHost()
//    }

//    buttonSendData.onClicked: {
//        userInterface.onRequestSweepSpectr()
//    }

//    buttonConnect.onClicked: {
//        userInterface.host = textEditAddress.text
//        userInterface.port = textEditPort.text
//        userInterface.onConnectToHost()
//    }
}
