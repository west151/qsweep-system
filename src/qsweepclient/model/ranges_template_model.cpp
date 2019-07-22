#include "ranges_template_model.h"

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

ranges_template_model::ranges_template_model(QObject *parent) : QAbstractListModel(parent)
{

}

QVariant ranges_template_model::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if ((index.row() >= 0) && (index.row() < m_data.size()))
        switch (role) {
        case DATAROLE_NAME:
            return m_data.at(index.row()).name_ranges();
        case DATAROLE_DESCR:
            return m_data.at(index.row()).descr_ranges();
        }

    return QVariant();
}

int ranges_template_model::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return m_data.size();
    else
        return 0;
}

QHash<int, QByteArray> ranges_template_model::roleNames() const
{
    QHash<int,QByteArray> roles;

    roles[DATAROLE_NAME] = "name_ranges";
    roles[DATAROLE_DESCR] = "descr_ranges";

    return roles;
}

void ranges_template_model::get_ranges_template_by_index(const int &index)
{

}

void ranges_template_model::add_result(const ranges_template &data)
{
    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
    m_data << data;
    endInsertRows();
}
