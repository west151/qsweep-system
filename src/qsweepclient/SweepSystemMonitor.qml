import QtQuick 2.4

SweepSystemMonitorForm {

    textHostName {
        text: qsTr("host name: ") + systemMonitorInterface.hostName
    }

    textUptime {
        text: qsTr("uptime: ") + systemMonitorInterface.uptime
    }

    textTotalMemory {
        text: qsTr("total memory: ") + (systemMonitorInterface.totalMemory/1024).toFixed(1) + qsTr(" Мб")
    }

    textFreeMemory {
        text: qsTr("free memory: ") + (systemMonitorInterface.freeMemory/1024).toFixed(1) + qsTr(" Мб")
    }

    textBufferMemory {
        text: qsTr("buffer memory: ") + (systemMonitorInterface.bufferMemory/1024).toFixed(1) + qsTr(" Мб")
    }
}
