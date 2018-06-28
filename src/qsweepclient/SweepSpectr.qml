import QtQuick 2.4
import QtCharts 2.2

SweepSpectrForm {


        Text {
            id: name
            text: qsTr("text")
            //anchors.fill: frameChart
//            anchors.top: frameChart.top
//            anchors.bottom: frameChart.bottom
//            anchors.right: frameChart.right
//            anchors.left: frameChart.right
        }

//    ChartView {
//        //id: chartViewSpectr
//        title: "Line"
//        //anchors.fill: frameChart
//        antialiasing: true

//        animationOptions: ChartView.NoAnimation
//        theme: ChartView.ChartThemeDark
//        property bool openGL: true
//        property bool openGLSupported: true

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
//    }


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
