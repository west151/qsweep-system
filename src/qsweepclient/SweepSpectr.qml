import QtQuick 2.4
import QtQuick.Controls 2.3
import QtCharts 2.2

SweepSpectrForm {

    ChartView {
        id: chartViewSpectr
        parent: frameChart
        anchors.fill: parent
        //title: "Line"
        antialiasing: true

        animationOptions: ChartView.NoAnimation
        theme: ChartView.ChartThemeDark
        property bool openGL: true
        property bool openGLSupported: true

        ValueAxis {
            id: axisY1
            min: -90
            max: 90
        }

        ValueAxis {
            id: axisY2
            min: -10
            max: 5
        }

        ValueAxis {
            id: axisX
            min: 0
            max: 1024.1
        }

        LineSeries {
            id: lineSeries1
            name: "signal 1"
            axisX: axisX
            axisY: axisY1
            useOpenGL: chartViewSpectr.openGL
        }


        Timer {
            id: refreshTimer
            interval: 1 / 60 * 1000 // 60 Hz
            running: false
            repeat: true
            onTriggered: {
                dataSource.update(chartViewSpectr.series(0));
                //dataSource.update(chartView.series(1));
            }
        }

//        LineSeries {
//            name: "LineSeries"
//            XYPoint { x: 0; y: 0 }
//            XYPoint { x: 1; y: 2.1 }
//            XYPoint { x: 2; y: 3.3 }
//            XYPoint { x: 3; y: 2.1 }
//            XYPoint { x: 4; y: 4.9 }
//            XYPoint { x: 5; y: 3.0 }
//            XYPoint { x: 6; y: 3.3 }
//        }
    }

    Connections {
        target: userInterface

        onSendStartSpectr: {
            console.log(qsTr("start Spectr"))
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

    textInputLNAGain {
        // RX LNA (IF) gain, 0-40dB, 8dB steps
        validator : IntValidator{bottom: 0; top: 40;}
    }

    textInputVGAGain {
        // RX VGA (baseband) gain, 0-62dB, 2dB steps
        validator : IntValidator{bottom: 0; top: 62;}
    }

    switchOneShot {
        enabled: false
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

    textInputLNAGain {
        enabled: false
    }

    textInputVGAGain {
        enabled: false
    }

    btnStart.onClicked: {
        userInterface.frequencyMin = textInputFreqMin.text
        userInterface.frequencyMax = textInputFreqMax.text
        userInterface.lnaGain = textInputLNAGain.text
        userInterface.vgaGain = textInputVGAGain.text
        userInterface.fftBinWidth = textInputFFTBinWidth.text
        userInterface.oneShot = switchOneShot.checkable
        userInterface.onRequestSweepSpectr()
    }
}
