#include "qsweepmessagelog.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QUuid>

QSweepMessageLog::QSweepMessageLog(QObject *parent) : QObject(parent)
{
    m_id = QUuid::createUuid().toString().mid(1, 8);
}

QString QSweepMessageLog::textMessage() const
{
    return m_textMessage;
}

void QSweepMessageLog::setTextMessage(const QString &textMessage)
{
    m_textMessage = textMessage;

    emit textMessageChanged(m_textMessage);
}

QByteArray QSweepMessageLog::exportToJson() const
{
    QJsonObject jsonObject;
    jsonObject["ID"] = m_id;
    jsonObject["TextMessage"] = m_textMessage;

    QJsonDocument doc(jsonObject);

    return doc.toJson(QJsonDocument::Compact);
}

QByteArray QSweepMessageLog::exportToJsonBinary() const
{
    QJsonObject jsonObject;
    jsonObject["ID"] = m_id;
    jsonObject["TextMessage"] = m_textMessage;

    QJsonDocument doc(jsonObject);

    return doc.toBinaryData();
}
