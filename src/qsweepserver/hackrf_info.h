#ifndef HACKRF_INFO_H
#define HACKRF_INFO_H

#include <QObject>

#include "hackrf.h"

class hackrf_info : public QObject
{
    Q_OBJECT
public:
    explicit hackrf_info(QObject *parent = nullptr);

public slots:
    void slot_run_hackrf_info(const QByteArray &value);

signals:
    void signal_hackrf_info(const QByteArray &value);

private:
    int result = HACKRF_SUCCESS;
    uint8_t board_id = BOARD_ID_INVALID;
    char version[255 + 1];
    uint16_t usb_version;
    read_partid_serialno_t read_partid_serialno;
    uint8_t operacakes[8];
    hackrf_device_list_t *list;
    hackrf_device* device;

    void error_hackrf(const QString &, int result);
    int get_hackrf_info(const QByteArray &);
};

#endif // HACKRF_INFO_H
