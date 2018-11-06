#ifndef MESSAGELOGMODEL_H
#define MESSAGELOGMODEL_H

#include <QtCore/qabstractitemmodel.h>
#include "data_log.h"

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
    void add_result(const data_log &data);
    void setMaxSize(const qint32 &);

private:
    QList<data_log> m_data;
    qint32 m_maxSize;
};

#endif // MESSAGELOGMODEL_H
