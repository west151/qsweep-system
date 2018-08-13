import QtQuick 2.4

SweepSystemMonitorForm {

    textHostName {
        text: "host name: " + systemMonitorInterface.hostName
    }

    textUptime {
        text: "uptime: " + systemMonitorInterface.uptime
    }

    textTotalMemory {
        text: "total memory: "+ (systemMonitorInterface.totalMemory/1024).toFixed(1) + " Мб"
    }

    textFreeMemory {
        text: "free memory: "+ (systemMonitorInterface.freeMemory/1024).toFixed(1) + " Мб"
    }

    textBufferMemory {
        text: "buffer memory: "+ (systemMonitorInterface.bufferMemory/1024).toFixed(1) + " Мб"
    }
}
