#ifndef RANGES_TEMPLATE_MODEL_H
#define RANGES_TEMPLATE_MODEL_H

#include <QtCore/qabstractitemmodel.h>
#include "template/ranges_template.h"

class ranges_template_model : public QAbstractListModel
{
    Q_OBJECT
public:
    enum DataRole {
        DATAROLE_NAME = Qt::UserRole + 1,
        DATAROLE_DESCR
    };
    explicit ranges_template_model(QObject* parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QHash< int, QByteArray > roleNames() const Q_DECL_OVERRIDE;

    Q_INVOKABLE void get_ranges_template_by_index(const int &index);
    void add_result(const ranges_template &data);

private:
    QVector<ranges_template> m_data;
};

#endif // RANGES_TEMPLATE_MODEL_H
