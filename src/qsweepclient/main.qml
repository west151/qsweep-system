import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.VirtualKeyboard 2.3

ApplicationWindow {
    id: window
    objectName: "mainWindow"
    visible: true
    width: 800
    height: 600
    title: qsTr("Spectr")

    Top{
        id: idTopForm
        objectName: "topForm"
        width: window.width
    }

    SwipeView {
        id: swipeView
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: parent.top
        anchors.topMargin: idTopForm.height

        currentIndex: tabBar.currentIndex

        SweepSpectr {
        }

        SweepSystemMonitor {
        }

        onCurrentIndexChanged: {
//            console.log("SwipeView: currentIndex = ", currentIndex);
        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex

        TabButton {
            text: qsTr("Spectr")
        }

        TabButton {
            text: qsTr("System monitor (remote host)")
        }
    }

    InputPanel {
        id: inputPanel
        z: 99
        x: 0
        y: window.height
        width: window.width

        states: State {
            name: "visible"
            when: inputPanel.active
            PropertyChanges {
                target: inputPanel
                y: window.height - inputPanel.height
            }
        }
        transitions: Transition {
            from: ""
            to: "visible"
            reversible: true
            ParallelAnimation {
                NumberAnimation {
                    properties: "y"
                    duration: 250
                    easing.type: Easing.InOutQuad
                }
            }
        }
    }
}
