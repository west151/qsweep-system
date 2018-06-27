import QtQuick 2.4

TopForm {

    textTime.text: Qt.formatTime(new Date(),"hh:mm")

    // обновлять системное время
    Timer {
        id: timer
        interval: 60000
        repeat: true
        running: true

        onTriggered:
        {
            textTime.text = Qt.formatTime(new Date(),"hh:mm")
        }
    }
}
