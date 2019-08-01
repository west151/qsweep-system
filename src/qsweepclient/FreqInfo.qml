import QtQuick 2.4

FreqInfoForm {

    list_view_freq_info {
        clip: true
        focus: true
        model: rangesTemplateModel
        delegate: viewInfoDelegate
    }

    Component {
        id: viewInfoDelegate
        Item {
            id: mainItem
            x: 5
            width: list_view_freq_info.width
            height: idTextInfo.height

            Row {
                id: idRow
                anchors.verticalCenter: parent.verticalCenter

                Text {
                    id: idTextInfo
                    width: mainItem.width
                    text: "Name: " + name_ranges + "\n"
                          + "Descr: " + descr_ranges + "\n"
                          + "Ranges: " + freq_ranges + "\n"
                    wrapMode: Text.WordWrap
                    anchors.verticalCenter: parent.verticalCenter
                    font.pointSize: 13
                }
                spacing: 10
            }
        }
    }
}
