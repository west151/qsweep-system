import QtQuick 2.4

TopForm {

//    labelHostBroker.text: userInterface.hostBroker
//    labelPortBroker.text: userInterface.portBroker
//    labelPingReceivedCount.text: userInterface.pingReceivedCount

//    labelBroker {
//        text: userInterface.hostBroker+":"+userInterface.portBroker+"["+userInterface.pingReceivedCount+"]"
//    }

    textTime {
        text: top_model.system_time
    }
}
