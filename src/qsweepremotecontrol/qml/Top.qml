import QtQuick 2.4

TopForm {

    Rectangle {
        id: idRectangleState
        width: textTime.height
        height: textTime.height
         x: 6
         anchors.verticalCenter: parent.verticalCenter
         color: "red"
         border.color: "red"
         border.width: 2
         radius: width*0.5
    }

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
