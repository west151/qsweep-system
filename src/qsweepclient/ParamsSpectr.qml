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
            height: idGridParams.height + 20
            // text params
            Rectangle{
                id: paramsRectangle
                x: 5
                y: 5
                width: mainItem.width - 10
                height: mainItem.height - 10
                color: "yellow"
                opacity: 0.5

                MouseArea {
                    anchors.fill: paramsRectangle
                    onClicked: {
                        listViewParamsSpectr.currentIndex = index
                    }
                }

                Grid {
                    id: idGridParams
                    width: mainItem.width
                    anchors.verticalCenter: parent.verticalCenter
                    x: 5
                    columns: 5
                    rows: 2
                    spacing: 2

                    // columns 1.1
                    Rectangle {
                        Text {
                            id: idTextMinFreq
                            text: "min freq"
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            font.pointSize: 13
                        }
                        width: idTextMinFreq.width + 10
                        height: idTextMinFreq.height
                    }
                    // columns 2.1
                    Rectangle {
                        Text {
                            id: idTextMaxFreq
                            text: "max freq"
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            font.pointSize: 13
                        }
                        width: idTextMaxFreq.width + 10
                        height: idTextMaxFreq.height
                    }
                    // columns 3.1
                    Rectangle {
                        Text {
                            id: idTextLNA
                            text: "lna"
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            font.pointSize: 13
                        }
                        width: idTextLNA.width + 10
                        height: idTextLNA.height
                    }
                    // columns 4.1
                    Rectangle {
                        Text {
                            id: idTextVGA
                            text: "vga"
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            font.pointSize: 13
                        }
                        width: idTextVGA.width + 10
                        height: idTextVGA.height
                    }
                    // columns 5.1
                    Rectangle {
                        Text {
                            id: idTextFFT
                            text: "fft width"
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            font.pointSize: 13
                        }
                        width: idTextFFT.width + 10
                        height: idTextFFT.height
                    }

                    // columns 1.2
                    Rectangle {
                        Text {
                            id: idMinFreq
                            text: freq_min
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            font.pointSize: 13
                        }
                        width: idTextMinFreq.width + 10
                        height: idTextMinFreq.height
                    }
                    // columns 2.2
                    Rectangle {
                        Text {
                            id: idMaxFreq
                            text: freq_max
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            font.pointSize: 13
                        }
                        width: idTextMaxFreq.width + 10
                        height: idTextMaxFreq.height
                    }
                    // columns 3.2
                    Rectangle {
                        Text {
                            id: idLNA
                            text: lna_gain
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            font.pointSize: 13
                        }
                        width: idTextLNA.width + 10
                        height: idTextLNA.height
                    }
                    // columns 4.2
                    Rectangle {
                        Text {
                            id: idVGA
                            text: vga_gain
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            font.pointSize: 13
                        }
                        width: idTextVGA.width + 10
                        height: idTextVGA.height
                    }
                    // columns 5.2
                    Rectangle {
                        Text {
                            id: idFFT
                            text: fft_bin_width
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            font.pointSize: 13
                        }
                        width: idTextFFT.width + 10
                        height: idTextFFT.height
                    }
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
