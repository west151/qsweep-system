#include "messagelogmodel.h"
#include <QtCore/QDateTime>

MessageLogModel::MessageLogModel(QObject *parent): QAbstractListModel(parent)
{
     m_data.clear();
     m_maxSize = 20;
}

QHash<int, QByteArray> MessageLogModel::roleNames() const
{
    QHash<int,QByteArray> roles;

    roles[DATAROLE_MESSAGE_TYPE] = "message_type";
    roles[DATAROLE_MESSAGE_TEXT] = "message_text";
    roles[DATAROLE_DT_MESSAGE] = "dt_message";

    return roles;
}

int MessageLogModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return m_data.size();
    else
        return 0;
}

void MessageLogModel::clearResult()
{
    if (m_data.size() > 0) {
        beginRemoveRows(QModelIndex(), 0, m_data.size() - 1);
        m_data.clear();
        endRemoveRows();
    }
}

void MessageLogModel::addResult(const QSweepMessageLog &data)
{
    if(m_data.size() >= m_maxSize)
        clearResult();

    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
    m_data << data;
    endInsertRows();
}

void MessageLogModel::setMaxSize(const qint32 &value)
{
    m_maxSize = value;
}

QVariant MessageLogModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if ((index.row() >= 0) && (index.row() < m_data.size()))
        switch (role) {
//        case DATAROLE_MESSAGE_TYPE:
//            return m_data.at(index.row()).messageLogType();
        case DATAROLE_DT_MESSAGE:
            return m_data.at(index.row()).dateTime().toUTC().toString("hh:mm:ss.zzz");
        case DATAROLE_MESSAGE_TEXT:
            return m_data.at(index.row()).textMessage();
        }

    return QVariant();
}
