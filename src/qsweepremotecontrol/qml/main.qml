import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.VirtualKeyboard 2.3

ApplicationWindow {
    id: id_window
    objectName: "mainWindow"
    visible: true
    width: 500
    height: 700
    title: qsTr("Remote control")

    Top{
        id: id_top_form
        objectName: "topForm"
        width: id_window.width
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
        anchors.topMargin: id_top_form.height

        currentIndex: tabBar.currentIndex

        SpectrFreqParams {
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

    }
}
