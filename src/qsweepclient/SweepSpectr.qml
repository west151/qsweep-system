import QtQuick 2.4
import QtQuick.Controls 2.3
import QtCharts 2.2
import waterfall 1.0

SweepSpectrForm {

    sliderSensitivity
    {
        id: idSliderSensitivity
        from: 0.005
        value: 0.05
        to: 0.1
        stepSize: 0.0001
    }

    btnClearMaxSpectr.onClicked: {
        userInterface.onClearMaxPowerSpectr()
    }

    rangeSliderLevel {
        from: -100
        to: 100
        first.value: -100
        second.value: 0
    }

    rangeSliderLevel.first.onValueChanged: valueAxisY.min = rangeSliderLevel.first.value
    rangeSliderLevel.second.onValueChanged: valueAxisY.max = rangeSliderLevel.second.value

    Waterfall {
        id: idPlot
        objectName: "plotWaterfall"
        parent: idFrameWaterfall
        anchors.fill: parent
        anchors.margins: 10
        sensitivity: idSliderSensitivity.value

        Rectangle {
            color: "#88222222"
            radius: 5
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.margins: 10
            width: minFreq.width + 10
            height: minFreq.height + 10

            Text {
                id: minFreq
                anchors.centerIn: parent
                color: "#fff"
                text: "← " + userInterface.frequencyMin + " MHz"
            }
        }

        Rectangle {
            color: "#88222222"
            radius: 5
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 10
            width: maxFreq.width + 10
            height: maxFreq.height + 10

            Text {
                id: maxFreq
                anchors.centerIn: parent
                color: "#fff"
                text: userInterface.frequencyMax + " MHz →"
            }
        }
    }

    frameWaterfall {
        id: idFrameWaterfall
        objectName: "frameWaterfall"
    }

    ChartView {
        id: chartViewSpectr
        objectName: "chartViewSpectr"
        parent: frameChart
        anchors.fill: parent
        //title: "Line"
        antialiasing: true

        animationOptions: ChartView.NoAnimation
        theme: ChartView.ChartThemeDark
        property bool openGL: true
        property bool openGLSupported: true

        LineSeries {
            id: lineSeriesPower
            objectName: "lineSeriesPower"
            name: "spectr"
            axisX: valueAxisX
            axisY: valueAxisY
            useOpenGL: chartViewSpectr.openGL
            color: "green"
            width: 0.9
        }

        LineSeries {
            id: lineSeriesPowerMax
            objectName: "lineSeriesPowerMax"
            name: "max spectr"
            axisX: valueAxisX
            axisY: valueAxisY
            useOpenGL: chartViewSpectr.openGL
            color: "red"
            width: 0.9
        }

//        Timer {
//            id: refreshTimer
//            interval: 1 / 60 * 1000 // 60 Hz
//            running: false
//            repeat: true
//            onTriggered: {
//                //dataSource.update(series)
//                //dataSource.update(chartViewSpectr.series(0));
//                //dataSource.update(chartView.series(1));
//            }
//        }
    }

    Connections {
        target: userInterface

        onSendStartSpectr: {
            //console.log(qsTr("start Spectr"))
            //refreshTimer.running = true
            dataSource.update(chartViewSpectr.series(0));
            dataSource.update(chartViewSpectr.series(1));
        }
    }

    textInputFreqMin{
        //validator : RegExpValidator { regExp : /[0-9]+\.[0-9]+/ }
        validator : IntValidator{bottom: 30; top: 6000;}
        inputMethodHints: Qt.ImhDigitsOnly
    }

    textInputFreqMax{
        validator : IntValidator{bottom: 30; top: 6000;}
        inputMethodHints: Qt.ImhDigitsOnly
    }

    textInputFFTBinWidth {
        enabled: false
    }

    // start spectr
    btnStart.onClicked: {
        userInterface.frequencyMin = textInputFreqMin.text
        userInterface.frequencyMax = textInputFreqMax.text
        userInterface.lnaGain = cbxLNAGain.currentText
        userInterface.vgaGain = cbxVGAGain.currentText
        userInterface.fftBinWidth = textInputFFTBinWidth.text
        userInterface.oneShot = switchOneShot.checked
        userInterface.onRequestSweepSpectr(true)
    }
    // stop spectr (stream)
    btnStop.onClicked: {
        userInterface.onRequestSweepSpectr(false)
    }

    cbxVGAGain {
        model: vgaGainModel
        currentIndex: 0
    }

    // RX VGA (baseband) gain, 0-62dB, 2dB steps
    ListModel {
        id: vgaGainModel
        Component.onCompleted:
        {
            for (var i = 0; i <= 62; i=i+2) {
                vgaGainModel.append({"text":i})
            }
        }
    }

    cbxLNAGain {
        model: lnaGainModel
        currentIndex: 0
    }

    // RX LNA (IF) gain, 0-40dB, 8dB steps
    ListModel {
        id: lnaGainModel
        ListElement { text: "0" }
        ListElement { text: "8" }
        ListElement { text: "16" }
        ListElement { text: "24" }
        ListElement { text: "32" }
        ListElement { text: "40" }
    }

}
