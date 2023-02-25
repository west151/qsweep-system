#include "message_log_model.h"
#include <QtCore/QDateTime>

message_log_model::message_log_model(QObject *parent): QAbstractListModel(parent)
{
     m_data.clear();
     m_maxSize = 20;
}

QHash<int, QByteArray> message_log_model::roleNames() const
{
    QHash<int,QByteArray> roles;

    roles[DATAROLE_MESSAGE_TYPE] = "message_type";
    roles[DATAROLE_MESSAGE_TEXT] = "message_text";
    roles[DATAROLE_DT_MESSAGE] = "dt_message";

    return roles;
}

int message_log_model::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return m_data.size();
    else
        return 0;
}

void message_log_model::clear_result()
{
    if (m_data.size() > 0) {
        beginRemoveRows(QModelIndex(), 0, m_data.size() - 1);
        m_data.clear();
        endRemoveRows();
    }
}

void message_log_model::add_result(const data_log &data)
{
    if(m_data.size() >= m_maxSize)
        clear_result();

    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
    m_data << data;
    endInsertRows();
}

void message_log_model::set_max_size(const qint32 &value)
{
    m_maxSize = value;
}

QVariant message_log_model::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if ((index.row() >= 0) && (index.row() < m_data.size()))
        switch (role) {
//        case DATAROLE_MESSAGE_TYPE:
//            return m_data.at(index.row()).messageLogType();
        case DATAROLE_DT_MESSAGE:
            return m_data.at(index.row()).date_time().toUTC().toString("hh:mm:ss.zzz");
        case DATAROLE_MESSAGE_TEXT:
            return m_data.at(index.row()).text_message();
        }

    return QVariant();
}
