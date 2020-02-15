#include "hackrf_info.h"

#include "sweep_message.h"
#include "sdr_info.h"

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

hackrf_info::hackrf_info(QObject *parent) : QObject(parent)
{

}

void hackrf_info::slot_run_hackrf_info(const QByteArray &value)
{
    get_hackrf_info(value);
}

int hackrf_info::get_hackrf_info(const QByteArray &value)
{
    Q_UNUSED(value)

    result = hackrf_init();

    if (result != HACKRF_SUCCESS) {
        error_hackrf(tr("hackrf_init() failed:"), result);
        return EXIT_FAILURE;
    }

    list = hackrf_device_list();

    if (list->devicecount < 1 ) {
#ifdef QT_DEBUG
        qDebug() << tr("No HackRF boards found.");
#endif
        return EXIT_FAILURE;
    }

    for (int i = 0; i < list->devicecount; i++)
    {
        sweep_message answer_data;
        answer_data.set_type(type_message::data_sdr_info);
        sdr_info hackrf_info;

        hackrf_info.set_index_board(i);

        // Serial number
        if (list->serial_numbers[i])
            hackrf_info.set_serial_numbers(QString(list->serial_numbers[i]));

        device = NULL;
        result = hackrf_device_list_open(list, i, &device);
        if (result != HACKRF_SUCCESS) {
            error_hackrf(tr("hackrf_open() failed:"), result);
            return EXIT_FAILURE;
        }

        // Board ID Number
        result = hackrf_board_id_read(device, &board_id);
        if (result == HACKRF_SUCCESS) {
            QString boardID(QString::number(board_id));
            boardID.append(tr(" (%1)").arg(hackrf_board_id_name(static_cast<hackrf_board_id>(board_id))));
            hackrf_info.set_board_id(boardID);
        }else{
            error_hackrf(tr("hackrf_board_id_read() failed:"), result);
            return EXIT_FAILURE;
        }

        // Firmware Version
        result = hackrf_version_string_read(device, &version[0], 255);
        if (result != HACKRF_SUCCESS) {
            error_hackrf(tr("hackrf_version_string_read() failed:"), result);
            return EXIT_FAILURE;
        }else{
            result = hackrf_usb_api_version_read(device, &usb_version);
            if (result == HACKRF_SUCCESS) {
                QString firmwareVer(version);
                firmwareVer.append(tr("(API:%1.%2)").arg((usb_version>>8)&0xFF).arg(usb_version&0xFF));
                hackrf_info.set_firmware_version(firmwareVer);
            }else{
                error_hackrf(tr("hackrf_usb_api_version_read() failed:"), result);
                return EXIT_FAILURE;
            }
        }

        result = hackrf_board_partid_serialno_read(device, &read_partid_serialno);

        if (result == HACKRF_SUCCESS)
        {
            QString partIDNumber;
            partIDNumber.append("0x");
            partIDNumber.append(QString::number(read_partid_serialno.part_id[0], 16));
            partIDNumber.append(" 0x");
            partIDNumber.append(QString::number(read_partid_serialno.part_id[1], 16));
            hackrf_info.set_part_id_number(partIDNumber);
        }else{
            error_hackrf(tr("hackrf_board_partid_serialno_read() failed:"), result);
            return EXIT_FAILURE;
        }

        result = hackrf_get_operacake_boards(device, &operacakes[0]);

        if ((result != HACKRF_SUCCESS) && (result != HACKRF_ERROR_USB_API_VERSION)) {
            error_hackrf(tr("hackrf_get_operacake_boards() failed:"), result);
            return EXIT_FAILURE;
        }

        if(result == HACKRF_SUCCESS) {
            for(int j=0; j<8; j++) {
                if(operacakes[j] == 0)
                    break;
#ifdef QT_DEBUG
                qDebug() << tr("Operacake found, address: 0x%1").arg(QString::number(operacakes[j], 16));
#endif
            }
        }

        result = hackrf_close(device);
        if (result != HACKRF_SUCCESS)
            error_hackrf(tr("hackrf_close() failed:"), result);

        QString libStr;
        libStr.append(hackrf_library_release());
        libStr.append("(");
        libStr.append(hackrf_library_version());
        libStr.append(")");
        hackrf_info.set_lib_sdr_version(libStr);

#ifdef QT_DEBUG
        qDebug() << tr("Serial number:") << hackrf_info.serial_numbers();
        qDebug() << tr("Board ID Number:") << hackrf_info.board_id();
        qDebug() << tr("Firmware Version:") << hackrf_info.firmware_version();
        qDebug() << tr("Part ID Number:") << hackrf_info.part_id_number();
        qDebug() << tr("Libhackrf Version:") << hackrf_info.lib_sdr_version();
#endif
        answer_data.set_data_message(hackrf_info.to_json());
        emit signal_hackrf_info(answer_data.to_json());
    }

    hackrf_device_list_free(list);
    hackrf_exit();

    return EXIT_SUCCESS;
}

void hackrf_info::error_hackrf(const QString &text, int result)
{
#ifdef QT_DEBUG
    qDebug() << text
             << hackrf_error_name(static_cast<hackrf_error>(result))
             << tr("(%1)").arg(result);
#endif
}
