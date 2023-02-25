import QtQuick 2.4

SpectrFreqParamsForm {

    cbx_lna_gain {
        textRole: "title"
        model: lna_gain_model

        onActivated: {
            console.log("lna_gain_model value:", lna_gain_model.get_lna_gain_value_by_index(cbx_lna_gain.currentIndex))
        }
    }

    cbx_vga_gain {
        textRole: "title"
        model: vga_gain_model

        onActivated: {
            console.log("vga_gain_model value:", vga_gain_model.get_lna_gain_value_by_index(cbx_lna_gain.currentIndex))
        }
    }

    cbx_fft_width {
        textRole: "title"
        model: fft_width_model

        onActivated: {
            console.log("fft_width_model value:", fft_width_model.get_fft_width_value_by_index(cbx_lna_gain.currentIndex))
        }
    }

    in_freq_min{
        //validator : RegExpValidator { regExp : /[0-9]+\.[0-9]+/ }
        validator : IntValidator{bottom: 1; top: 6000;}
        inputMethodHints: Qt.ImhDigitsOnly
    }

    in_freq_max{
        validator : IntValidator{bottom: 30; top: 6000;}
        inputMethodHints: Qt.ImhDigitsOnly
    }

    // fft_width_model

//    cbx_lna_gain {
//        textRole: "title"
//        model: lna_gain_model
//        delegate: Rectangle {
//            height: cbx_lna_gain.height
//            width: cbx_lna_gain.width
//            Text {
//                x: 10
//                anchors.verticalCenter: parent.verticalCenter
//                font: cbx_lna_gain.font
//                text: title
//            }
//            MouseArea {
//                anchors.fill: parent
//                onClicked: {
//                    console.log("cbx_lna_gain title:", title)
//                    console.log("cbx_lna_gain value:", lna_gain)
//                    console.log("cbx_lna_gain index:", index)
//                    cbx_lna_gain.popup.close()

//                    if (index !== cbx_lna_gain.selectedIndex) {
//                        //cbx_lna_gain.selectedIndex = index
//                        comboIndexChanged(index)
//                    }
//                }
//            }
//        }
//        onActivated: {
//            //console.log("cbx_lna_gain index:", cbx_lna_gain.currentText, lna_gain_model.get(cbx_lna_gain.currentIndex).value)
//            console.log("cbx_lna_gain index:", cbx_lna_gain.currentText)
//        }
//    }

}
