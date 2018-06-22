import QtQuick 2.4

ClientForm {

    buttonDisconnect.onClicked: {
        userInterface.onDisconnectFromHost()
    }

    buttonConnect.onClicked: {
        userInterface.host = textEditAddress.text
        userInterface.port = textEditPort.text
        userInterface.onConnectToHost()
    }
}
