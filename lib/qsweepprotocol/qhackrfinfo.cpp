#include "qhackrfinfo.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QUuid>

static const QString INDEX_KEY = QStringLiteral("index");
static const QString SERIAL_NUMBERS_KEY = QStringLiteral("serial");
static const QString BOARD_ID_KEY = QStringLiteral("board_id");

QHackrfInfo::QHackrfInfo(QObject *parent) : QObject(parent)
{
    m_valid = false;
    m_index = -1;
    m_serialNumbers.clear();
    m_boardID.clear();
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

QByteArray QHackrfInfo::exportToJson() const
{
    QJsonObject jsonObject;
    jsonObject[INDEX_KEY] = m_index;
    jsonObject[SERIAL_NUMBERS_KEY] = m_serialNumbers;
    jsonObject[BOARD_ID_KEY] = m_boardID;

    QJsonDocument doc(jsonObject);

    return doc.toJson(QJsonDocument::Compact);
}

QByteArray QHackrfInfo::exportToJsonBinary() const
{
    QJsonObject jsonObject;
    jsonObject[INDEX_KEY] = m_index;
    jsonObject[SERIAL_NUMBERS_KEY] = m_serialNumbers;
    jsonObject[BOARD_ID_KEY] = m_boardID;

    QJsonDocument doc(jsonObject);

    return doc.toBinaryData();
}
