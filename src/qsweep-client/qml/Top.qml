import QtQuick 2.4

TopForm {

//    labelHostBroker.text: userInterface.hostBroker
//    labelPortBroker.text: userInterface.portBroker
//    labelPingReceivedCount.text: userInterface.pingReceivedCount

    labelBroker {
        text: userInterface.hostBroker+":"+userInterface.portBroker+"["+userInterface.pingReceivedCount+"]"
    }

    textTime.text: Qt.formatTime(new Date(),"hh:mm")

    // обновлять системное время
    Timer {
        id: timer
        interval: 60000
        repeat: true
        running: true

        onTriggered:
        {
            textTime.text = Qt.formatTime(new Date(),"hh:mm")
        }
    }
}
