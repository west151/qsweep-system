#include "params_spectr_model.h"

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

params_spectr_model::params_spectr_model(QObject *parent) : QAbstractListModel(parent)
{

}

QVariant params_spectr_model::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if ((index.row() >= 0) && (index.row() < m_data.size()))
        switch (role) {
        case DATAROLE_LNA_GAIN:
            return m_data.at(index.row()).lna_gain();
        case DATAROLE_VGA_GAIN:
            return m_data.at(index.row()).vga_gain();
        case DATAROLE_FFT_BIN_WIDTH:
            return m_data.at(index.row()).fft_bin_width();
        case DATAROLE_FREQ_MIN:
            return m_data.at(index.row()).frequency_min();
        case DATAROLE_FREQ_MAX:
            return m_data.at(index.row()).frequency_max();
        }

    return QVariant();
}

int params_spectr_model::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return m_data.size();
    else
        return 0;
}

bool params_spectr_model::removeRows(int row, int count, const QModelIndex &index)
{
    beginRemoveRows(index, row, row + count - 1);
    m_data.clear();
    endRemoveRows();

    return true;
}

QHash<int, QByteArray> params_spectr_model::roleNames() const
{
    QHash<int,QByteArray> roles;

    roles[DATAROLE_LNA_GAIN] = "lna_gain";
    roles[DATAROLE_VGA_GAIN] = "vga_gain";
    roles[DATAROLE_FFT_BIN_WIDTH] = "fft_bin_width";
    roles[DATAROLE_FREQ_MIN] = "freq_min";
    roles[DATAROLE_FREQ_MAX] = "freq_max";

    return roles;
}

void params_spectr_model::clear_result_all()
{
    if (m_data.size() > 0) {
        beginRemoveRows(QModelIndex(), 0, m_data.size() - 1);
        m_data.clear();
        endRemoveRows();
    }
}

void params_spectr_model::clear_result(const params_spectr &data)
{
    Q_UNUSED(data)
}

void params_spectr_model::clear_result_by_index(const int &index)
{
    if(index < 0 || index >= m_data.size())
        return;

    emit signal_remove_from_model(m_data.at(index).id_params());

    beginRemoveRows(QModelIndex(), index, index);
    m_data.erase(m_data.begin() + index);
    endRemoveRows();
}

void params_spectr_model::get_params_by_index(const int &index)
{
    if(index < 0 || index >= m_data.size())
        return;

    emit signal_params_from_model(m_data.at(index));
}

void params_spectr_model::add_result(const params_spectr &data)
{
    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
    m_data << data;
    endInsertRows();
}

void params_spectr_model::slot_set_vector_result(const QVector<params_spectr> &data)
{
    if (m_data.size() > 0) {
        beginRemoveRows(QModelIndex(), 0, m_data.size() - 1);
        m_data.clear();
        endRemoveRows();
    }
    if (data.size() > 0) {
        beginInsertRows(QModelIndex(), 0, data.size() - 1);
        m_data = data;
        endInsertRows();
    }
}

void params_spectr_model::slot_remove_from_model(const QString &id_params)
{
    if ((m_data.size() > 0)&&(!id_params.isEmpty()))
    {

    }
}
