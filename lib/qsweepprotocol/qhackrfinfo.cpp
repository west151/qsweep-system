#include "qhackrfinfo.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

#include "constkeys.h"

class QHackrfInfoData : public QSharedData {
public:
    QHackrfInfoData(): QSharedData()
    {
        m_serialNumbers.clear();
        m_index = -1;
        m_boardID.clear();
        m_firmwareVersion.clear();
        m_partIDNumber.clear();
        m_libHackrfVersion.clear();
        m_valid = false;
    }
    QHackrfInfoData(const QHackrfInfoData &other) : QSharedData(other)
    {
        m_serialNumbers = other.m_serialNumbers;
        m_index = other.m_index;
        m_boardID = other.m_boardID;
        m_firmwareVersion = other.m_firmwareVersion;
        m_partIDNumber = other.m_partIDNumber;
        m_libHackrfVersion = other.m_libHackrfVersion;
        m_valid = other.m_valid;
    }

    ~QHackrfInfoData() {}

    bool m_valid;
    qint32 m_index;
    QString m_serialNumbers;
    QString m_boardID;
    QString m_firmwareVersion;
    QString m_partIDNumber;
    QString m_libHackrfVersion;
};

QHackrfInfo::QHackrfInfo() : data(new QHackrfInfoData)
{
}

QHackrfInfo::QHackrfInfo(const QHackrfInfo &rhs) : data(rhs.data)
{
}

QHackrfInfo::QHackrfInfo(const QByteArray &json, const bool binary) : data(new QHackrfInfoData)
{
    QJsonDocument doc;
    if (binary)
        doc = QJsonDocument::fromBinaryData(json, QJsonDocument::BypassValidation);
    else
        doc = QJsonDocument::fromJson(json);

    QJsonObject jsonObject = doc.object();
    data->m_index = jsonObject.value(INDEX_KEY).toInt();
    data->m_serialNumbers = jsonObject.value(SERIAL_NUMBERS_KEY).toString();
    data->m_boardID = jsonObject.value(BOARD_ID_KEY).toString();
    data->m_firmwareVersion = jsonObject.value(FIRMWARE_VERSION_KEY).toString();
    data->m_partIDNumber = jsonObject.value(PART_ID_NUMBER_KEY).toString();
    data->m_libHackrfVersion = jsonObject.value(LIB_HACKRF_VERSION_KEY).toString();

    if(!doc.isEmpty())
        data->m_valid = true;
    else
        data->m_valid = false;
}

QHackrfInfo &QHackrfInfo::operator=(const QHackrfInfo &rhs)
{
    if (this != &rhs) {
        data.operator=(rhs.data);
    }
    return *this;
}

QHackrfInfo::~QHackrfInfo()
{
}

void QHackrfInfo::setIndexBoard(const qint32 &value)
{
    data->m_index = value;
}

qint32 QHackrfInfo::indexBoard() const
{
    return data->m_index;
}

void QHackrfInfo::setSerialNumbers(const QString &value)
{
    data->m_serialNumbers = value;
}

QString QHackrfInfo::serialNumbers() const
{
    return data->m_serialNumbers;
}

void QHackrfInfo::setBoardID(const QString &value)
{
    data->m_boardID = value;
}

QString QHackrfInfo::boardID() const
{
    return data->m_boardID;
}

void QHackrfInfo::setFirmwareVersion(const QString &value)
{
    data->m_firmwareVersion = value;
}

QString QHackrfInfo::firmwareVersion() const
{
    return data->m_firmwareVersion;
}

void QHackrfInfo::setPartIDNumber(const QString &value)
{
    data->m_partIDNumber = value;
}

QString QHackrfInfo::partIDNumber() const
{
    return data->m_partIDNumber;
}

void QHackrfInfo::setLibHackrfVersion(const QString &value)
{
    data->m_libHackrfVersion = value;
}

QString QHackrfInfo::libHackrfVersion() const
{
    return data->m_libHackrfVersion;
}

bool QHackrfInfo::isValid() const
{
    return data->m_valid;
}

QByteArray QHackrfInfo::exportToJson(const bool binary) const
{
    QJsonObject jsonObject;
    jsonObject.insert(INDEX_KEY, data->m_index);
    jsonObject.insert(SERIAL_NUMBERS_KEY, data->m_serialNumbers);
    jsonObject.insert(BOARD_ID_KEY, data->m_boardID);
    jsonObject.insert(FIRMWARE_VERSION_KEY, data->m_firmwareVersion);
    jsonObject.insert(PART_ID_NUMBER_KEY, data->m_partIDNumber);
    jsonObject.insert(LIB_HACKRF_VERSION_KEY, data->m_libHackrfVersion);

    QJsonDocument doc(jsonObject);

    if(binary)
        return doc.toBinaryData();
    else
        return doc.toJson(QJsonDocument::Compact);
}
