#ifndef SDR_INFO_H
#define SDR_INFO_H

#include <QSharedData>
#include <QMetaType>

class sdr_info_data;

class sdr_info
{
public:
    sdr_info();
    sdr_info(const sdr_info &);
    sdr_info(const QByteArray &json);
    sdr_info &operator=(const sdr_info &);
    ~sdr_info();

    void set_index_board(const qint32 &);
    qint32 index_board()const;

    void set_serial_numbers(const QString &);
    QString serial_numbers()const;

    void set_board_id(const QString &);
    QString board_id()const;

    void set_firmware_version(const QString &);
    QString firmware_version()const;

    void set_part_id_number(const QString &);
    QString part_id_number()const;

    void set_lib_sdr_version(const QString &);
    QString lib_sdr_version() const;

    bool is_valid() const;

    QByteArray to_json() const;

private:
    QSharedDataPointer<sdr_info_data> data;
};
Q_DECLARE_METATYPE(sdr_info)

#endif // SDR_INFO_H
