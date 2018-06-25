#include "hackrfinfomodel.h"

HackrfInfoModel::HackrfInfoModel(QObject *parent): QAbstractListModel(parent)
{

}

QVariant HackrfInfoModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if ((index.row() >= 0) && (index.row() < m_data.size()))
        switch (role) {
        case DATAROLE_INDEX_BOARD:
            return m_data.at(index.row())->indexBoard();
        case DATAROLE_SERIAL_NUMBERS:
            return m_data.at(index.row())->serialNumbers();
        case DATAROLE_BOARD_ID:
            return m_data.at(index.row())->boardID();
        case DATAROLE_FIRMWARE_VERSION:
            return m_data.at(index.row())->firmwareVersion();
        case DATAROLE_PART_ID_NUMBER:
            return m_data.at(index.row())->partIDNumber();
        case DATAROLE_HACKRF_VERSION:
            return m_data.at(index.row())->libHackrfVersion();
        }

    return QVariant();
}

int HackrfInfoModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return m_data.size();
    else
        return 0;
}

QHash<int, QByteArray> HackrfInfoModel::roleNames() const
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

void HackrfInfoModel::clearResult()
{
    if (m_data.size() > 0) {
        beginRemoveRows(QModelIndex(), 0, m_data.size() - 1);
        m_data.clear();
        endRemoveRows();
    }
}

void HackrfInfoModel::addResult(QHackrfInfo *data)
{
    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());
    m_data << data;
    endInsertRows();
}
