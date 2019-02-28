import QtQuick 2.4

ParamsSpectrForm {

    listViewParamsSpectr {
        clip: true
        focus: true
        model: paramsSpectrModel
        delegate: viewParamsDelegate
//        highlight: Rectangle {
//            color: "grey"
//        }
    }

    listViewParamsSpectr.onCurrentItemChanged: console.log('selected')

    Component {
        id: viewParamsDelegate
        Item {
            id: mainItem
            width: listViewParamsSpectr.width
            height: idTextInfo.height + 20

            Rectangle{
                id: paramsRectangle
                width: mainItem.width - 50
                height: mainItem.height
                border.color: "black"
                color: "green"

                Row {
                    id: idRow
                    anchors.verticalCenter: parent.verticalCenter

                    Text {
                        id: idTextInfo
                        width: mainItem.width
                        text: "min:" + freq_min + " max:" + freq_max
                              + " lna:" + lna_gain
                              + " vga:" + vga_gain
                              + " fft width:" + fft_bin_width
                        wrapMode: Text.WordWrap
                        anchors.verticalCenter: parent.verticalCenter
                        font.pointSize: 13
                    }
                    spacing: 10
                }
            }
            MouseArea {
                anchors.fill: paramsRectangle
                onClicked: listViewParamsSpectr.currentIndex = index
            }

            Rectangle{
                x: mainItem.width - 45
                width: mainItem.width - paramsRectangle.width - 5
                height: mainItem.height
                border.color: "black"
                color: "red"
            }
        }        
    }
}
