import QtQuick 2.4
import QtQuick.Controls 2.3

Item {
    id: item_main
    width: 800
    height: 600
    property alias check_box_db_write: check_box_db_write
    property alias button_save: button_save
    property alias button_params: button_params
    property alias check_box_max_spectr: check_box_max_spectr
    property alias button_start: button_start
    property alias button_stop: button_stop
    property alias in_freq_max: in_freq_max
    property alias in_freq_min: in_freq_min
    property alias cbx_fft_size: cbx_fft_size
    property alias cbx_vga_gain: cbx_vga_gain
    property alias cbx_lna_gain: cbx_lna_gain
    property alias item_spectr_surface: item_spectr_surface

    Item {
        id: item_spectr_surface
        anchors.right: parent.right
        anchors.rightMargin: 174
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 8
    }

    Item {
        id: element
        x: 632
        width: 160
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 8

        ComboBox {
            id: cbx_lna_gain
            x: 44
            y: 108
            width: 108
            height: 40
        }

        TextField {
            id: in_freq_min
            x: 8
            y: 8
            width: 65
            height: 40
            text: qsTr("2300")
        }

        TextField {
            id: in_freq_max
            x: 87
            y: 8
            width: 65
            height: 40
            text: qsTr("2700")
        }

        ComboBox {
            id: cbx_fft_size
            x: 8
            y: 58
            width: 144
            height: 40
        }

        ComboBox {
            id: cbx_vga_gain
            x: 44
            y: 160
            width: 108
            height: 40
        }

        Label {
            id: label
            x: 12
            y: 117
            text: qsTr("lna")
            font.pointSize: 14
        }

        Label {
            id: label1
            x: 8
            y: 169
            text: qsTr("vga")
            font.pointSize: 14
        }

        Button {
            id: button_start
            y: 536
            width: 65
            height: 40
            text: qsTr("start")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 8
            anchors.left: parent.left
            anchors.leftMargin: 8
        }

        Button {
            id: button_stop
            x: 87
            y: 536
            width: 65
            height: 40
            text: qsTr("stop")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 8
            anchors.right: parent.right
            anchors.rightMargin: 8
        }

        CheckBox {
            id: check_box_max_spectr
            x: 8
            y: 257
            width: 144
            height: 40
            text: qsTr("max spectr")
        }

        Button {
            id: button_params
            x: 8
            y: 211
            width: 65
            height: 40
            text: qsTr("Profile")
        }

        Button {
            id: button_save
            x: 87
            y: 211
            width: 65
            height: 40
            text: qsTr("Save")
            checked: false
        }

        CheckBox {
            id: check_box_db_write
            x: 8
            y: 296
            text: qsTr("spectr write")
        }

    }
}
