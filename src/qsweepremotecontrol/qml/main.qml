import QtQuick 2.10
import QtQuick.Controls 2.3
import QtQuick.VirtualKeyboard 2.3

ApplicationWindow {
    id: window
    objectName: "mainWindow"
    visible: true
    width: 500
    height: 700
    title: qsTr("Remote control")

    Top{
        id: idTopForm
        objectName: "topForm"
        width: window.width
    }
}
