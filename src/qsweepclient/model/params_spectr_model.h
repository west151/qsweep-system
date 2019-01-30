#ifndef PARAMS_SPECTR_MODEL_H
#define PARAMS_SPECTR_MODEL_H

#include <QtCore/qabstractitemmodel.h>
#include "params_spectr.h"

class params_spectr_model : public QAbstractListModel
{
    Q_OBJECT
public:
    enum DataRole {
        DATAROLE_LNA_GAIN = Qt::UserRole + 1,
        DATAROLE_VGA_GAIN,
        DATAROLE_FFT_BIN_WIDTH,
        DATAROLE_FREQ_MIN,
        DATAROLE_FREQ_MAX
    };

    explicit params_spectr_model(QObject* parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QHash< int, QByteArray > roleNames() const Q_DECL_OVERRIDE;

    void clear_result_all();
    void clear_result(const params_spectr &data);
    void add_result(const params_spectr &data);

public slots:
    void slot_set_vector_result(const QVector<params_spectr> &data);

private:
    QVector<params_spectr> m_data;
};

#endif // PARAMS_SPECTR_MODEL_H
