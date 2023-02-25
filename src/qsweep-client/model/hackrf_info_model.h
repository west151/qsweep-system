#ifndef HACKRF_INFO_MODEL_H
#define HACKRF_INFO_MODEL_H

#include <QtCore/qabstractitemmodel.h>
#include "sdr_info.h"

class hackrf_info_model : public QAbstractListModel
{
    Q_OBJECT
public:
    enum DataRole {
        DATAROLE_INDEX_BOARD = Qt::UserRole + 1,
        DATAROLE_SERIAL_NUMBERS,
        DATAROLE_BOARD_ID,
        DATAROLE_FIRMWARE_VERSION,
        DATAROLE_PART_ID_NUMBER,
        DATAROLE_HACKRF_VERSION
    };

    explicit hackrf_info_model(QObject* parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QHash< int, QByteArray > roleNames() const Q_DECL_OVERRIDE;

    void clear_result();
    void add_result(const sdr_info &);

private:
    QList<sdr_info> m_data;
};

#endif // HACKRF_INFO_MODEL_H
