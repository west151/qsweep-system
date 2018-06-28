import QtQuick 2.4

ClientForm {

    labelHostBroker.text: userInterface.hostBroker
    labelPortBroker.text: userInterface.portBroker
    labelPingReceivedCount.text: userInterface.pingReceivedCount


//    buttonDisconnect.onClicked: {
//        userInterface.onDisconnectFromHost()
//    }

//    buttonConnect.onClicked: {
//        userInterface.host = textEditAddress.text
//        userInterface.port = textEditPort.text
//        userInterface.onConnectToHost()
//    }
}
