#ifndef MESSAGE_LOG_MODEL_H
#define MESSAGE_LOG_MODEL_H

#include <QAbstractListModel>
#include "data_log.h"

class message_log_model : public QAbstractListModel
{
    Q_OBJECT
public:
    enum DataRole {
        DATAROLE_MESSAGE_TYPE = Qt::UserRole + 1,
        DATAROLE_DT_MESSAGE,
        DATAROLE_MESSAGE_TEXT
    };

    explicit message_log_model(QObject* parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QHash< int, QByteArray > roleNames() const Q_DECL_OVERRIDE;

    void clear_result();
    void add_result(const data_log &data);
    void set_max_size(const qint32 &);

private:
    QList<data_log> m_data;
    qint32 m_maxSize;
};

#endif // MESSAGE_LOG_MODEL_H
