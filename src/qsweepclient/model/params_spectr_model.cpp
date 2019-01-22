#include "params_spectr_model.h"

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

void params_spectr_model::add_result(const params_spectr &data)
{
    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
    m_data << data;
    endInsertRows();
}
