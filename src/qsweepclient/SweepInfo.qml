import QtQuick 2.4

SweepInfoForm {
    buttonUpdate.onClicked: {
        userInterface.onRequestSweepInfo()
    }

    listView.model: hackrfInfoModel;
    listView.delegate: viewDelegate

    Component {
        id: viewDelegate
        Item {
            id: mainItem
            x: 5
            width: listView.width
            height: 50

            Row {
                id: row
                anchors.verticalCenter: parent.verticalCenter

//                Text {
//                    text: message_type
//                    color: "#ffffff"
//                    anchors.verticalCenter: parent.verticalCenter
//                    font.pointSize: 13
//                }

                Text {
                    //anchors.centerIn: parent
                    width: mainItem.width
                    //text: "info_serial_numbers"
                    //text: hackrfInfoModel.
                    wrapMode: Text.WordWrap
                    color: "#ffffff"
                    anchors.verticalCenter: parent.verticalCenter
                    font.pointSize: 13

                }
                spacing: 10
            }
        }
    }

//        // делегат для отображения "контента"
//        Component {
//            id: viewDelegate
//            Item {
//                id: mainItem
//                x: 5
//                width: listView.width
//                height: 50

//                Row {
//                    id: row
//                    anchors.verticalCenter: parent.verticalCenter

//                    Text {
//                        text: message_type
//                        color: "#ffffff"
//                        anchors.verticalCenter: parent.verticalCenter
//                        font.pointSize: 13
//                    }

//                    Text {
//                        //anchors.centerIn: parent
//                        width: mainItem.width
//                        text: message_text
//                        wrapMode: Text.WordWrap
//                        color: "#ffffff"
//                        anchors.verticalCenter: parent.verticalCenter
//                        font.pointSize: 13

//                    }
//                    spacing: 10
//                }
//            }
//        }
}
