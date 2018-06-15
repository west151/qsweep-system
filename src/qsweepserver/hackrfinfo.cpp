#include "hackrfinfo.h"

#include "qsweepanswer.h"
#include "qhackrfinfo.h"

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

HackrfInfo::HackrfInfo(QObject *parent) : QObject(parent)
{

}

int HackrfInfo::getHackrfInfo()
{
    result = hackrf_init();

    if (result != HACKRF_SUCCESS) {
#ifdef QT_DEBUG
        qDebug() << tr("hackrf_init() failed:") << hackrf_error_name(static_cast<hackrf_error>(result))
                 << tr("(%1)").arg(result);
#endif
        return EXIT_FAILURE;
    }

#ifdef QT_DEBUG
    qDebug() << tr("libhackrf version:") << hackrf_library_release()
             << tr("(%1)").arg(hackrf_library_version());
#endif

    list = hackrf_device_list();

    if (list->devicecount < 1 ) {
#ifdef QT_DEBUG
        qDebug() << tr("No HackRF boards found.");
#endif
        return EXIT_FAILURE;
    }

    for (int i = 0; i < list->devicecount; i++)
    {
        QSweepAnswer answerData;
        answerData.setTypeAnswer(TypeAnswer::INFO);
        QHackrfInfo info;

        info.setIndexBoard(i);

        // Serial number
        if (list->serial_numbers[i])
            info.setSerialNumbers(QString(list->serial_numbers[i]));

        device = NULL;
        result = hackrf_device_list_open(list, i, &device);
        if (result != HACKRF_SUCCESS) {
            errorHackrf(tr("hackrf_open() failed:"), result);
            return EXIT_FAILURE;
        }

        // Board ID Number
        result = hackrf_board_id_read(device, &board_id);
        if (result == HACKRF_SUCCESS) {
            QString boardID(QString::number(board_id));
            boardID.append(tr(" (%1)").arg(hackrf_board_id_name(static_cast<hackrf_board_id>(board_id))));
            info.setBoardID(boardID);
        }else{
            errorHackrf(tr("hackrf_board_id_read() failed:"), result);
            return EXIT_FAILURE;
        }

        // Firmware Version
        result = hackrf_version_string_read(device, &version[0], 255);
        if (result != HACKRF_SUCCESS) {
            errorHackrf(tr("hackrf_version_string_read() failed:"), result);
            return EXIT_FAILURE;
        }

        result = hackrf_usb_api_version_read(device, &usb_version);
        if (result != HACKRF_SUCCESS) {
            errorHackrf(tr("hackrf_usb_api_version_read() failed:"), result);
            return EXIT_FAILURE;
        }

#ifdef QT_DEBUG
        qDebug() << tr("Firmware Version:") << version
                 << tr("(API:%1.%2)").arg((usb_version>>8)&0xFF).arg(usb_version&0xFF);
#endif


        result = hackrf_board_partid_serialno_read(device, &read_partid_serialno);

        if (result != HACKRF_SUCCESS)
        {

#ifdef QT_DEBUG
            qDebug() << tr("hackrf_board_partid_serialno_read() failed:") << hackrf_error_name(static_cast<hackrf_error>(result))
                     << tr("(%1)").arg(result);
#endif

            return EXIT_FAILURE;
        }
        //        printf("Part ID Number: 0x%08x 0x%08x\n",
        //                    read_partid_serialno.part_id[0],
        //                    read_partid_serialno.part_id[1]);

#ifdef QT_DEBUG
        qDebug() << tr("Part ID Number:") << read_partid_serialno.part_id[0] << read_partid_serialno.part_id[1];
#endif

        result = hackrf_get_operacake_boards(device, &operacakes[0]);

        if ((result != HACKRF_SUCCESS) && (result != HACKRF_ERROR_USB_API_VERSION))
        {

#ifdef QT_DEBUG
            qDebug() << tr("hackrf_get_operacake_boards() failed:") << hackrf_error_name(static_cast<hackrf_error>(result))
                     << tr("(%1)").arg(result);
#endif

            return EXIT_FAILURE;
        }
        if(result == HACKRF_SUCCESS) {
            for(int j=0; j<8; j++) {
                if(operacakes[j] == 0)
                    break;
                // printf("Operacake found, address: 0x%02x\n", operacakes[j]);
#ifdef QT_DEBUG
                qDebug() << tr("Operacake found, address:") << operacakes[j];
#endif
            }
        }

        result = hackrf_close(device);
        if (result != HACKRF_SUCCESS)
        {
#ifdef QT_DEBUG
            qDebug() << tr("hackrf_close() failed:") << hackrf_error_name(static_cast<hackrf_error>(result))
                     << tr("(%1)").arg(result);
#endif
        }

#ifdef QT_DEBUG
        qDebug() << tr("--------------------------- %1 -------------------------------").arg(i);
        qDebug() << tr("Serial number:") << info.serialNumbers();
        qDebug() << tr("Board ID Number:") << info.boardID();
        qDebug() << tr("--------------------------------------------------------------");
#endif

        answerData.setDataAnswer(info.exportToJson());
        sendHackrfInfo(answerData);
    }

    hackrf_device_list_free(list);
    hackrf_exit();

    return EXIT_SUCCESS;
}

void HackrfInfo::errorHackrf(const QString &text, int result)
{
#ifdef QT_DEBUG
    qDebug() << text
             << hackrf_error_name(static_cast<hackrf_error>(result))
             << tr("(%1)").arg(result);
#endif
}