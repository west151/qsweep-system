#ifndef MESSAGELOGMODEL_H
#define MESSAGELOGMODEL_H

#include <QtCore/qabstractitemmodel.h>
#include "qsweepmessagelog.h"

class MessageLogModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum DataRole {
        DATAROLE_MESSAGE_TYPE = Qt::UserRole + 1,
        DATAROLE_DT_MESSAGE,
        DATAROLE_MESSAGE_TEXT
    };

    explicit MessageLogModel(QObject* parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QHash< int, QByteArray > roleNames() const Q_DECL_OVERRIDE;

    void clearResult();
    void addResult(const QSweepMessageLog &data);

private:
    QList<QSweepMessageLog> m_data;
};

#endif // MESSAGELOGMODEL_H
