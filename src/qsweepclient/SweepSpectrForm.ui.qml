import QtQuick 2.4
import QtQuick.Controls 2.3

Item {
    width: 640
    height: 480
    property alias textInputFreqMax: textInputFreqMax
    property alias textInputFreqMin: textInputFreqMin

    Frame {
        id: frame
        anchors.fill: parent

        Frame {
            id: frameChart
            anchors.right: parent.right
            anchors.rightMargin: 198
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.top: parent.top

            Label {
                id: label
                x: 391
                y: 59
                text: qsTr("Label")
                anchors.right: parent.right
                anchors.rightMargin: 0
            }
        }

        Frame {
            id: frameParams
            x: 424
            width: 192
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0

            Label {
                id: label1
                y: 396
                text: qsTr("Label")
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 19
                anchors.left: parent.left
                anchors.leftMargin: 0
            }

            TextField {
                id: textInputFreqMin
                x: 111
                y: 0
                width: 57
                height: 40
                text: qsTr("2300")
            }

            TextField {
                id: textInputFreqMax
                x: 111
                y: 52
                width: 57
                height: 40
                text: qsTr("2700")
            }

            Label {
                id: label2
                x: 80
                y: 12
                text: qsTr("min")
            }

            Label {
                id: label3
                x: 77
                y: 64
                text: qsTr("max")
            }
        }
    }
}
