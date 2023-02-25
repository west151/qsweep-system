import QtQuick 2.4
import QtQuick.Controls 2.13

Item {
    id: item_main
    width: 500
    height: 700
    property alias btn_ctrl_spectr: btn_ctrl_spectr
    property alias cbx_vga_gain: cbx_vga_gain
    property alias cbx_lna_gain: cbx_lna_gain
    property alias cbx_fft_width: cbx_fft_width
    property alias in_freq_max: in_freq_max
    property alias in_freq_min: in_freq_min

    Item {
        id: element
        x: 8
        width: 484
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 384
        anchors.top: parent.top
        anchors.topMargin: 8

        TextField {
            id: in_freq_min
            x: 8
            y: 8
            width: 150
            height: 40
            text: qsTr("1")
            font.pointSize: 15
            horizontalAlignment: Text.AlignHCenter
        }

        TextField {
            id: in_freq_max
            x: 326
            y: 8
            width: 150
            height: 40
            text: qsTr("6000")
            font.pointSize: 15
            horizontalAlignment: Text.AlignHCenter
        }

        CheckBox {
            id: cbx_lock_freq_band
            x: 197
            y: 8
            width: 90
            height: 40
            text: qsTr("lock")
            font.pointSize: 15
        }

        RangeSlider {
            id: range_slider_freq
            x: 9
            y: 64
            width: 468
            height: 40
            stepSize: 5
            to: 6000
            from: 1
            second.value: 6000
            first.value: 1
        }

        ComboBox {
            id: cbx_fft_width
            x: 263
            y: 119
            width: 213
            height: 40
            font.pointSize: 15
        }

        ComboBox {
            id: cbx_lna_gain
            x: 336
            y: 165
            font.pointSize: 15
        }

        ComboBox {
            id: cbx_vga_gain
            x: 336
            y: 211
            font.pointSize: 15
        }

        Label {
            id: label_fft_widt
            x: 177
            y: 128
            text: qsTr("fft width")
            font.pointSize: 15
        }

        Label {
            id: label_lna_gain
            x: 242
            y: 174
            text: qsTr("lna gain")
            font.pointSize: 15
        }

        Label {
            id: label_vga_gain
            x: 242
            y: 220
            text: qsTr("vga gain")
            font.pointSize: 15
        }

        Button {
            id: btn_ctrl_spectr
            x: 336
            y: 257
            width: 140
            height: 40
            text: qsTr("run")
            font.pointSize: 15
        }
    }
}
