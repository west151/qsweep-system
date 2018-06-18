#include "qhackrfinfo.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QUuid>

static const QString INDEX_KEY = QStringLiteral("index");
static const QString SERIAL_NUMBERS_KEY = QStringLiteral("serial");
static const QString BOARD_ID_KEY = QStringLiteral("board_id");
static const QString FIRMWARE_VERSION_KEY = QStringLiteral("firmware_version");
static const QString PART_ID_NUMBER_KEY = QStringLiteral("part_id_number");
static const QString LIB_HACKRF_VERSION_KEY = QStringLiteral("lib_hackrf_version");

QHackrfInfo::QHackrfInfo(QObject *parent) : QObject(parent)
{
    m_valid = false;
    m_index = -1;
    m_serialNumbers.clear();
    m_boardID.clear();
    m_firmwareVersion.clear();
    m_partIDNumber.clear();
    m_libHackrfVersion.clear();
}

QHackrfInfo::QHackrfInfo(const QByteArray &json, bool binary)
{
    QJsonDocument doc;
    if (binary)
        doc = QJsonDocument::fromBinaryData(json, QJsonDocument::BypassValidation);
    else
        doc = QJsonDocument::fromJson(json);

    QJsonObject jsonObject = doc.object();
    m_index = jsonObject[INDEX_KEY].toInt();
    m_serialNumbers = jsonObject[SERIAL_NUMBERS_KEY].toString();
    m_boardID = jsonObject[BOARD_ID_KEY].toString();
    m_firmwareVersion = jsonObject[FIRMWARE_VERSION_KEY].toString();
    m_partIDNumber = jsonObject[PART_ID_NUMBER_KEY].toString();
    m_libHackrfVersion = jsonObject[LIB_HACKRF_VERSION_KEY].toString();

    if(!doc.isEmpty())
        m_valid = true;
    else
        m_valid = false;
}

bool QHackrfInfo::isValid() const
{
    return m_valid;
}

void QHackrfInfo::setIndexBoard(const qint32 &value)
{
    m_index = value;
}

qint32 QHackrfInfo::indexBoard() const
{
    return m_index;
}

void QHackrfInfo::setSerialNumbers(const QString &value)
{
    m_serialNumbers = value;
}

QString QHackrfInfo::serialNumbers() const
{
    return m_serialNumbers;
}

void QHackrfInfo::setBoardID(const QString &value)
{
    m_boardID = value;
}

QString QHackrfInfo::boardID() const
{
    return m_boardID;
}

void QHackrfInfo::setFirmwareVersion(const QString &value)
{
    m_firmwareVersion = value;
}

QString QHackrfInfo::firmwareVersion() const
{
    return m_firmwareVersion;
}

void QHackrfInfo::setPartIDNumber(const QString &value)
{
    m_partIDNumber = value;
}

QString QHackrfInfo::partIDNumber() const
{
    return m_partIDNumber;
}

void QHackrfInfo::setLibHackrfVersion(const QString &value)
{
    m_libHackrfVersion = value;
}

QString QHackrfInfo::libHackrfVersion() const
{
    return m_libHackrfVersion;
}

QByteArray QHackrfInfo::exportToJson() const
{
    QJsonObject jsonObject;
    jsonObject[INDEX_KEY] = m_index;
    jsonObject[SERIAL_NUMBERS_KEY] = m_serialNumbers;
    jsonObject[BOARD_ID_KEY] = m_boardID;
    jsonObject[FIRMWARE_VERSION_KEY] = m_firmwareVersion;
    jsonObject[PART_ID_NUMBER_KEY] = m_partIDNumber;
    jsonObject[LIB_HACKRF_VERSION_KEY] = m_libHackrfVersion;

    QJsonDocument doc(jsonObject);

    return doc.toJson(QJsonDocument::Compact);
}

QByteArray QHackrfInfo::exportToJsonBinary() const
{
    QJsonObject jsonObject;
    jsonObject[INDEX_KEY] = m_index;
    jsonObject[SERIAL_NUMBERS_KEY] = m_serialNumbers;
    jsonObject[BOARD_ID_KEY] = m_boardID;
    jsonObject[FIRMWARE_VERSION_KEY] = m_firmwareVersion;
    jsonObject[PART_ID_NUMBER_KEY] = m_partIDNumber;
    jsonObject[LIB_HACKRF_VERSION_KEY] = m_libHackrfVersion;

    QJsonDocument doc(jsonObject);

    return doc.toBinaryData();
}
