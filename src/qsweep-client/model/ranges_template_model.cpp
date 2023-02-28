#include "ranges_template_model.h"

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
        case DATAROLE_RANGES:
            return ranges_to_str_descr(m_data.at(index.row()).freq_ranges());
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
    roles[DATAROLE_RANGES] = "freq_ranges";

    return roles;
}

void ranges_template_model::get_ranges_template_by_index(const int &index)
{
    Q_UNUSED(index)
}

void ranges_template_model::add_result(const ranges_template &data)
{
    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
    m_data << data;
    endInsertRows();
}

QString ranges_template_model::ranges_to_str_descr(const QList<QPair<quint64, quint64> > &in_ranges) const
{
    QStringList str_descr;

    for(int i=0; i<in_ranges.size(); ++i)
    {
        QString tmp_str;

        tmp_str.append(tr("( "));
        tmp_str.append(QString::number(static_cast<double>(in_ranges.at(i).first/1e6),'f', 1));
        tmp_str.append(tr(" - "));
        tmp_str.append(QString::number(static_cast<double>(in_ranges.at(i).second/1e6),'f', 1));
        tmp_str.append(tr(" )"));

        str_descr.append(tmp_str);
    }

    return str_descr.join(",");
}
