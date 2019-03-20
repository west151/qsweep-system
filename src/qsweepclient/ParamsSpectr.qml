import QtQuick 2.4

ParamsSpectrForm {

    property int cIndex: listViewParamsSpectr.currentIndex

    listViewParamsSpectr {
        clip: true
        focus: true
        model: paramsSpectrModel
        delegate: viewParamsDelegate
        highlight: Rectangle {
            x: 0
            color: "grey"
        }
    }

    listViewParamsSpectr.onCurrentItemChanged: console.log('selected', listViewParamsSpectr.currentIndex)

    Component {
        id: viewParamsDelegate
        Item {
            id: mainItem
            width: listViewParamsSpectr.width
            height: idTextInfo.height + 20

            // text params
            Rectangle{
                id: paramsRectangle
                x: 5
                y: 5
                width: mainItem.width - 10
                height: mainItem.height - 10
                //border.color: "black"
                color: "yellow"

                MouseArea {
                    anchors.fill: paramsRectangle
                    onClicked: {
                        listViewParamsSpectr.currentIndex = index
                    }
                }

                Row {
                    id: idRow
                    //x: paramsApply.width + 5
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

                Rectangle{
                    id: paramsRemove
                    x: paramsRectangle.width - paramsRemove.width
                    width: paramsRectangle.height
                    height: paramsRectangle.height
                    color: "red"
                }

                MouseArea {
                    anchors.fill: paramsRemove
                    onClicked: {
                        listViewParamsSpectr.currentIndex = index
                        paramsSpectrModel.clear_result_by_index(index)
                    }
                }
            }
        }        
    }
}
