#ifndef fft_width_MODEL_H
#define fft_width_MODEL_H

#include <QAbstractListModel>
#include "fft_width.h"

class fft_width_model : public QAbstractListModel
{
    Q_OBJECT
public:
    enum data_role {
        DATAROLE_TITLE = Qt::UserRole + 1,
        DATAROLE_FFT_WIDTH
    };

    explicit fft_width_model(QObject* parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QHash< int, QByteArray > roleNames() const Q_DECL_OVERRIDE;

    void add_result(const QString &title, const quint32 &value);

    Q_INVOKABLE quint32 get_fft_width_value_by_index(const int &index);

private:
    QList<fft_width> m_data;
};

#endif // fft_width_MODEL_H
