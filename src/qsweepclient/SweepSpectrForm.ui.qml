import QtQuick 2.4
import QtQuick.Controls 2.3

Item {
    width: 800
    height: 600
    property alias sliderSensitivity: sliderSensitivity
    property alias frameWaterfall: frameWaterfall
    property alias btnClearMaxSpectr: btnClearMaxSpectr
    property alias rangeSliderLevel: rangeSliderLevel
    property alias btnStop: btnStop
    property alias cbxLNAGain: cbxLNAGain
    property alias cbxVGAGain: cbxVGAGain
    property alias frameChart: frameChart
    property alias textInputFFTBinWidth: textInputFFTBinWidth
    property alias switchOneShot: switchOneShot
    property alias btnStart: btnStart
    property alias textInputFreqMax: textInputFreqMax
    property alias textInputFreqMin: textInputFreqMin

    Frame {
        id: frame
        anchors.fill: parent

        Frame {
            id: frameChart
            anchors.bottomMargin: 276
            anchors.leftMargin: 49
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
                height: 40
                text: "2400"
                anchors.left: parent.left
                anchors.leftMargin: 111
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.top: parent.top
                anchors.topMargin: 0
            }

            TextField {
                id: textInputFreqMax
                x: 111
                width: 57
                height: 40
                text: qsTr("2500")
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.top: parent.top
                anchors.topMargin: 52
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
                x: 20
                y: 392
                width: 69
                height: 40
                text: qsTr("Run")
                anchors.right: parent.right
                anchors.rightMargin: 79
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
            }

            Label {
                id: label
                x: 32
                y: 118
                text: qsTr("LNA gain")
            }

            Label {
                id: label1
                x: 31
                y: 170
                text: qsTr("VGA gain")
            }

            Switch {
                id: switchOneShot
                x: 31
                y: 266
                text: qsTr("One Shot")
                autoExclusive: false
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
                width: 69
                height: 40
                text: "500000"
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.top: parent.top
                anchors.topMargin: 215
            }

            ComboBox {
                id: cbxVGAGain
                x: 99
                width: 70
                height: 40
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.top: parent.top
                anchors.topMargin: 158
                currentIndex: -1
            }

            Label {
                id: label4
                x: 1
                y: 226
                text: qsTr("FFT bin width")
            }

            ComboBox {
                id: cbxLNAGain
                x: 99
                y: 106
                width: 70
            }

            Button {
                id: btnStop
                x: 100
                y: 392
                width: 69
                height: 40
                text: qsTr("Stop")
                anchors.bottom: parent.bottom
                anchors.rightMargin: -1
                anchors.bottomMargin: 0
                anchors.right: parent.right
            }

            Button {
                id: btnClearMaxSpectr
                x: 26
                y: 312
                width: 148
                height: 40
                text: qsTr("clear max")
            }

        }

        RangeSlider {
            id: rangeSliderLevel
            anchors.top: parent.top
            anchors.topMargin: 50
            anchors.verticalCenterOffset: -138
            anchors.left: parent.left
            anchors.leftMargin: 3
            anchors.verticalCenter: parent.verticalCenter
            orientation: Qt.Vertical
            second.value: 0.75
            first.value: 0.25
        }

        Frame {
            id: frameWaterfall
            y: 306
            height: 270
            anchors.right: parent.right
            anchors.rightMargin: 198
            anchors.left: parent.left
            anchors.leftMargin: 49
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
        }

        Slider {
            id: sliderSensitivity
            y: 341
            to: 0.1
            from: 0
            anchors.left: parent.left
            anchors.leftMargin: 3
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 35
            orientation: Qt.Vertical
            value: 0.1
        }
    }
}

/*##^## Designer {
    D{i:4;anchors_width:57;anchors_x:111;anchors_y:0}D{i:5;anchors_y:52}D{i:9;anchors_y:106}
D{i:11;anchors_y:157}D{i:15;anchors_y:215}
}
 ##^##*/
