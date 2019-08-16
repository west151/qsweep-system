import QtQuick 2.4

SDRInfoForm {
    list_view_sdr_info {
        clip: true
        focus: true
        model: hackrfInfoModel
        delegate: viewInfoDelegate
    }

    Component {
        id: viewInfoDelegate
        Item {
            id: mainItem
            x: 5
            width: list_view_sdr_info.width
            height: idTextInfo.height

            Row {
                id: idRow
                anchors.verticalCenter: parent.verticalCenter

                Text {
                    id: idTextInfo
                    width: mainItem.width
                    text: "Serial Numbers: " + info_serial_numbers + "\n"
                          + "Board ID: " + info_board_id + "\n"
                          + "Firmware Version: " + info_firmware_version + "\n"
                          + "Part ID Number: " + info_part_id_number + "\n"
                          + "Libhackrf Version: " + info_hackrf_version
                    wrapMode: Text.WordWrap
                    anchors.verticalCenter: parent.verticalCenter
                    font.pointSize: 13
                }
                spacing: 10
            }
        }
    }
}
