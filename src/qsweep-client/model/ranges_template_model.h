#ifndef RANGES_TEMPLATE_MODEL_H
#define RANGES_TEMPLATE_MODEL_H

#include <QAbstractListModel>
#include "ranges_template.h"

class ranges_template_model : public QAbstractListModel
{
    Q_OBJECT
public:
    enum DataRole {
        DATAROLE_NAME = Qt::UserRole + 1,
        DATAROLE_DESCR,
        DATAROLE_RANGES
    };
    explicit ranges_template_model(QObject* parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QHash< int, QByteArray > roleNames() const Q_DECL_OVERRIDE;

    Q_INVOKABLE void get_ranges_template_by_index(const int &index);
    void add_result(const ranges_template &data);

private:
    QVector<ranges_template> m_data;

    QString ranges_to_str_descr(const QList<QPair<quint64, quint64> > &)const;
};

#endif // RANGES_TEMPLATE_MODEL_H
