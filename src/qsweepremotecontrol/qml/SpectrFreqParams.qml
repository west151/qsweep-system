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
