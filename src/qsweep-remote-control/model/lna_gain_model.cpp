#include "lna_gain_model.h"

lna_gain_model::lna_gain_model(QObject *parent): QAbstractListModel(parent)
{
    m_data.clear();
}

QVariant lna_gain_model::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if ((index.row() >= 0) && (index.row() < m_data.size()))
        switch (role) {
        case DATAROLE_TITLE:
            return m_data.at(index.row()).title();
        case DATAROLE_LNA_GAIN:
            return m_data.at(index.row()).lna_gain_value();
        }

    return QVariant();
}

int lna_gain_model::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return m_data.size();
    else
        return 0;
}

QHash<int, QByteArray> lna_gain_model::roleNames() const
{
    QHash<int,QByteArray> roles;

    roles[DATAROLE_TITLE] = "title";
    roles[DATAROLE_LNA_GAIN] = "lna_gain_value";

    return roles;
}

void lna_gain_model::add_result(const QString &title, const quint32 &value)
{
    lna_gain data;
    data.set_title(title);
    data.set_lna_gain_value(value);

    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
    m_data.append(data);
    endInsertRows();
}

quint32 lna_gain_model::get_lna_gain_value_by_index(const int &index)
{
    if(index < 0 || index >= m_data.size())
        return 0;

    return m_data.at(index).lna_gain_value();
}
