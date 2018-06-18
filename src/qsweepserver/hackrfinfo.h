#ifndef HACKRFINFO_H
#define HACKRFINFO_H

#include <QObject>

#include "hackrf.h"

class QSweepAnswer;

class HackrfInfo : public QObject
{
    Q_OBJECT
public:
    explicit HackrfInfo(QObject *parent = nullptr);

public slots:
    void onRunHackrfInfo(const QByteArray &value);

signals:
    void sendHackrfInfo(const QSweepAnswer &);
    //void sendHackrfInfo(const QByteArray &);

private:
    int result = HACKRF_SUCCESS;
    uint8_t board_id = BOARD_ID_INVALID;
    char version[255 + 1];
    uint16_t usb_version;
    read_partid_serialno_t read_partid_serialno;
    uint8_t operacakes[8];
    hackrf_device_list_t *list;
    hackrf_device* device;

    void errorHackrf(const QString &, int result);
    int hackrfInfo(const QByteArray &);
};

#endif // HACKRFINFO_H
