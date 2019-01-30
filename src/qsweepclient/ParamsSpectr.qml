import QtQuick 2.4

ParamsSpectrForm {

    listViewParamsSpectr.model: paramsSpectrModel
    listViewParamsSpectr.delegate: viewLogDelegate

    Component {
        id: viewLogDelegate
        Item {
            id: mainItem
            x: 5
            width: listViewParamsSpectr.width
            height: idTextInfo.height + 10

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
    }
}
