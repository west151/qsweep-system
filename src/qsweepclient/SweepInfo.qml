import QtQuick 2.4

SweepInfoForm {
    buttonUpdate.onClicked: {
        userInterface.onRequestSweepInfo()
    }
}
