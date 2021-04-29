#include "fft_width_model.h"

fft_width_model::fft_width_model(QObject *parent): QAbstractListModel(parent)
{
    m_data.clear();
}

QVariant fft_width_model::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if ((index.row() >= 0) && (index.row() < m_data.size()))
        switch (role) {
        case DATAROLE_TITLE:
            return m_data.at(index.row()).title();
        case DATAROLE_FFT_WIDTH:
            return m_data.at(index.row()).fft_width_value();
        }

    return QVariant();
}

int fft_width_model::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return m_data.size();
    else
        return 0;
}

QHash<int, QByteArray> fft_width_model::roleNames() const
{
    QHash<int,QByteArray> roles;

    roles[DATAROLE_TITLE] = "title";
    roles[DATAROLE_FFT_WIDTH] = "fft_width_value";

    return roles;
}

void fft_width_model::add_result(const QString &title, const quint32 &value)
{
    fft_width data;
    data.set_title(title);
    data.set_fft_width_value(value);

    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
    m_data.append(data);
    endInsertRows();
}

quint32 fft_width_model::get_fft_width_value_by_index(const int &index)
{
    if(index < 0 || index >= m_data.size())
        return 0;

    return m_data.at(index).fft_width_value();
}
