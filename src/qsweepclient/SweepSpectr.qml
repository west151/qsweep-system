import QtQuick 2.4
import QtQuick.Controls 2.3
import QtCharts 2.2

SweepSpectrForm {

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
//            name: "signal 1"
            axisX: valueAxisX
            axisY: valueAxisY
            useOpenGL: chartViewSpectr.openGL
        }

        Timer {
            id: refreshTimer
            interval: 1 / 60 * 1000 // 60 Hz
            running: false
            repeat: true
            onTriggered: {
                //dataSource.update(series)
                //dataSource.update(chartViewSpectr.series(0));
                //dataSource.update(chartView.series(1));
            }
        }
    }

    Connections {
        target: userInterface

        onSendStartSpectr: {
            //console.log(qsTr("start Spectr"))
            //refreshTimer.running = true
            dataSource.update(chartViewSpectr.series(0));
        }
    }

    textInputFreqMin{
        //validator : RegExpValidator { regExp : /[0-9]+\.[0-9]+/ }
        validator : IntValidator{bottom: 30; top: 6000;}
    }

    textInputFreqMax{
        validator : IntValidator{bottom: 30; top: 6000;}
    }

    textInputFFTBinWidth {
        enabled: false
    }

    textInputFreqMin {
        enabled: false
    }

    textInputFreqMax {
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
