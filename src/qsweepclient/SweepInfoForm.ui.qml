import QtQuick 2.4
import QtQuick.Controls 2.3

Item {
    width: 640
    height: 480
    property alias buttonUpdate: buttonUpdate

    Frame {
        id: frame
        anchors.fill: parent

        Frame {
            id: frameControl
            height: 54
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: parent.top

            Button {
                id: buttonUpdate
                y: -5
                width: 84
                height: 40
                text: qsTr("Update")
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Frame {
            id: frameInfo
            anchors.top: parent.top
            anchors.topMargin: 60
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.left: parent.left

            ListView {
                id: listView
                anchors.fill: parent
                model: ListModel {
                    ListElement {
                        name: "Grey"
                        colorCode: "grey"
                    }

                    ListElement {
                        name: "Red"
                        colorCode: "red"
                    }

                    ListElement {
                        name: "Blue"
                        colorCode: "blue"
                    }

                    ListElement {
                        name: "Green"
                        colorCode: "green"
                    }
                }
                delegate: Item {
                    x: 5
                    width: 80
                    height: 40
                    Row {
                        id: row1
                        Rectangle {
                            width: 40
                            height: 40
                            color: colorCode
                        }

                        Text {
                            text: name
                            anchors.verticalCenter: parent.verticalCenter
                            font.bold: true
                        }
                        spacing: 10
                    }
                }
            }
        }
    }
}
