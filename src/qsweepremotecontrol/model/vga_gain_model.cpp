#include "vga_gain_model.h"

vga_gain_model::vga_gain_model(QObject *parent): QAbstractListModel(parent)
{
    m_data.clear();
}

QVariant vga_gain_model::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if ((index.row() >= 0) && (index.row() < m_data.size()))
        switch (role) {
        case DATAROLE_TITLE:
            return m_data.at(index.row()).title();
        case DATAROLE_VGA_GAIN:
            return m_data.at(index.row()).vga_gain_value();
        }

    return QVariant();
}

int vga_gain_model::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return m_data.size();
    else
        return 0;
}

QHash<int, QByteArray> vga_gain_model::roleNames() const
{
    QHash<int,QByteArray> roles;

    roles[DATAROLE_TITLE] = "title";
    roles[DATAROLE_VGA_GAIN] = "vga_gain_value";

    return roles;
}

void vga_gain_model::add_result(const QString &title, const quint32 &value)
{
    vga_gain data;
    data.set_title(title);
    data.set_vga_gain_value(value);

    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
    m_data.append(data);
    endInsertRows();
}

quint32 vga_gain_model::get_lna_gain_value_by_index(const int &index)
{
    if(index < 0 || index >= m_data.size())
        return 0;

    return m_data.at(index).vga_gain_value();
}
