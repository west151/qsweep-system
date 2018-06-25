#ifndef HACKRFINFOMODEL_H
#define HACKRFINFOMODEL_H

#include <QtCore/qabstractitemmodel.h>
#include "qhackrfinfo.h"

class HackrfInfoModel : public QAbstractListModel
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

    explicit HackrfInfoModel(QObject* parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QHash< int, QByteArray > roleNames() const Q_DECL_OVERRIDE;

    void clearResult();
    void addResult(const QHackrfInfo &);

private:
    QList<QHackrfInfo> m_data;
};

#endif // HACKRFINFOMODEL_H
