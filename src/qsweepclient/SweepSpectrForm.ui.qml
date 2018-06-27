import QtQuick 2.4
import QtQuick.Controls 2.3

Item {
    width: 640
    height: 480
    property alias textInputFFTBinWidth: textInputFFTBinWidth
    property alias switchOneShot: switchOneShot
    property alias textInputVGAGain: textInputVGAGain
    property alias textInputLNAGain: textInputLNAGain
    property alias btnStart: btnStart
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
                x: 46
                y: 12
                text: qsTr("Freq min")
            }

            Label {
                id: label3
                x: 43
                y: 64
                text: qsTr("Freq max")
            }

            Button {
                id: btnStart
                x: 99
                y: 392
                width: 69
                height: 40
                text: qsTr("Run")
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
            }

            TextField {
                id: textInputLNAGain
                x: 111
                y: 106
                width: 57
                height: 40
                text: qsTr("0")
            }

            Label {
                id: label
                x: 44
                y: 118
                text: qsTr("LNA gain")
            }

            TextField {
                id: textInputVGAGain
                x: 111
                y: 157
                width: 57
                height: 40
                text: qsTr("0")
            }

            Label {
                id: label1
                x: 43
                y: 169
                text: qsTr("VGA gain")
            }

            Switch {
                id: switchOneShot
                x: 31
                y: 266
                text: qsTr("One Shot")
                wheelEnabled: false
                checked: true
            }

            Row {
                x: 13
                y: 203
            }

            TextField {
                id: textInputFFTBinWidth
                x: 99
                y: 215
                width: 69
                height: 40
                text: qsTr("500000")
            }

            Label {
                id: label4
                x: 1
                y: 226
                text: qsTr("FFT bin width")
            }
        }
    }
}
