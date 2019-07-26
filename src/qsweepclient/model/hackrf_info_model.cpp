#include "hackrf_info_model.h"

hackrf_info_model::hackrf_info_model(QObject *parent): QAbstractListModel(parent)
{
    m_data.clear();
}

QVariant hackrf_info_model::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if ((index.row() >= 0) && (index.row() < m_data.size())){
        switch (role) {
        case DATAROLE_INDEX_BOARD:
            return m_data.at(index.row()).index_board();
        case DATAROLE_SERIAL_NUMBERS:
            return m_data.at(index.row()).serial_numbers();
        case DATAROLE_BOARD_ID:
            return m_data.at(index.row()).board_id();
        case DATAROLE_FIRMWARE_VERSION:
            return m_data.at(index.row()).firmware_version();
        case DATAROLE_PART_ID_NUMBER:
            return m_data.at(index.row()).part_id_number();
        case DATAROLE_HACKRF_VERSION:
            return m_data.at(index.row()).lib_sdr_version();
        }
    }

    return QVariant();
}

int hackrf_info_model::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    else
        return m_data.size();

}

QHash<int, QByteArray> hackrf_info_model::roleNames() const
{
    QHash<int,QByteArray> roles;

    roles[DATAROLE_INDEX_BOARD] = "info_index_board";
    roles[DATAROLE_SERIAL_NUMBERS] = "info_serial_numbers";
    roles[DATAROLE_BOARD_ID] = "info_board_id";
    roles[DATAROLE_FIRMWARE_VERSION] = "info_firmware_version";
    roles[DATAROLE_PART_ID_NUMBER] = "info_part_id_number";
    roles[DATAROLE_HACKRF_VERSION] = "info_hackrf_version";

    return roles;
}

void hackrf_info_model::clear_result()
{
    if (m_data.size() > 0) {
        beginRemoveRows(QModelIndex(), 0, m_data.size() - 1);
        m_data.clear();
        endRemoveRows();
    }
}

void hackrf_info_model::add_result(const sdr_info &data)
{
    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
    m_data.append(data);
    endInsertRows();
}
