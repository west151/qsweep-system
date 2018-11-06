#include "sdr_info.h"

#include <QJsonDocument>
#include <QJsonObject>

#include "constkeys.h"

class sdr_info_data : public QSharedData {
public:
    sdr_info_data(): QSharedData()
    {
        m_serial_numbers.clear();
        m_index = -1;
        m_board_id.clear();
        m_firmware_version.clear();
        m_part_id_number.clear();
        m_lib_sdr_version.clear();
        m_valid = false;
    }
    sdr_info_data(const sdr_info_data &other) : QSharedData(other)
    {
        m_serial_numbers = other.m_serial_numbers;
        m_index = other.m_index;
        m_board_id = other.m_board_id;
        m_firmware_version = other.m_firmware_version;
        m_part_id_number = other.m_part_id_number;
        m_lib_sdr_version = other.m_lib_sdr_version;
        m_valid = other.m_valid;
    }

    ~sdr_info_data() {}

    qint32 m_index;
    QString m_serial_numbers;
    QString m_board_id;
    QString m_firmware_version;
    QString m_part_id_number;
    QString m_lib_sdr_version;
    bool m_valid;
};

sdr_info::sdr_info() : data(new sdr_info_data)
{
}

sdr_info::sdr_info(const sdr_info &rhs) : data(rhs.data)
{
}

sdr_info::sdr_info(const QByteArray &json, const bool binary) : data(new sdr_info_data)
{
    QJsonDocument doc;
    if (binary)
        doc = QJsonDocument::fromBinaryData(json, QJsonDocument::BypassValidation);
    else
        doc = QJsonDocument::fromJson(json);

    const QJsonObject json_object(doc.object());
    data->m_index = json_object.value(INDEX_KEY).toInt();
    data->m_serial_numbers = json_object.value(SERIAL_NUMBERS_KEY).toString();
    data->m_board_id = json_object.value(BOARD_ID_KEY).toString();
    data->m_firmware_version = json_object.value(FIRMWARE_VERSION_KEY).toString();
    data->m_part_id_number = json_object.value(PART_ID_NUMBER_KEY).toString();
    data->m_lib_sdr_version = json_object.value(LIB_HACKRF_VERSION_KEY).toString();

    if(!doc.isEmpty())
        data->m_valid = true;
    else
        data->m_valid = false;
}

sdr_info &sdr_info::operator=(const sdr_info &rhs)
{
    if (this != &rhs) {
        data.operator=(rhs.data);
    }
    return *this;
}

sdr_info::~sdr_info()
{
}

void sdr_info::set_index_board(const qint32 &value)
{
    data->m_index = value;
}

qint32 sdr_info::index_board() const
{
    return  data->m_index;
}

void sdr_info::set_serial_numbers(const QString &value)
{
    data->m_serial_numbers = value;
}

QString sdr_info::serial_numbers() const
{
    return data->m_serial_numbers;
}

void sdr_info::set_board_id(const QString &value)
{
    data->m_board_id = value;
}

QString sdr_info::board_id() const
{
    return  data->m_board_id;
}

void sdr_info::set_firmware_version(const QString &value)
{
    data->m_firmware_version = value;
}

QString sdr_info::firmware_version() const
{
    return  data->m_firmware_version;
}

void sdr_info::set_part_id_number(const QString &value)
{
    data->m_part_id_number = value;
}

QString sdr_info::part_id_number() const
{
    return data->m_part_id_number;
}

void sdr_info::set_lib_sdr_version(const QString &value)
{
    data->m_lib_sdr_version = value;
}

QString sdr_info::lib_sdr_version() const
{
    return  data->m_lib_sdr_version;
}

bool sdr_info::is_valid() const
{
    return data->m_valid;
}

QByteArray sdr_info::export_json(const bool binary) const
{
    QJsonObject json_object;
    json_object.insert(INDEX_KEY, data->m_index);
    json_object.insert(SERIAL_NUMBERS_KEY, data->m_serial_numbers);
    json_object.insert(BOARD_ID_KEY, data->m_board_id);
    json_object.insert(FIRMWARE_VERSION_KEY, data->m_firmware_version);
    json_object.insert(PART_ID_NUMBER_KEY, data->m_part_id_number);
    json_object.insert(LIB_HACKRF_VERSION_KEY, data->m_lib_sdr_version);

    const QJsonDocument doc(json_object);

    if(binary)
        return doc.toBinaryData();
    else
        return doc.toJson(QJsonDocument::Compact);
}
