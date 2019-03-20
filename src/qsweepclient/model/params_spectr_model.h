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
    bool removeRows(int row, int count, const QModelIndex &index) Q_DECL_OVERRIDE;
    QHash< int, QByteArray > roleNames() const Q_DECL_OVERRIDE;

    void clear_result_all();
    void clear_result(const params_spectr &data);
    Q_INVOKABLE void clear_result_by_index(const int &index);

    Q_INVOKABLE void get_params_by_index(const int &index);

    void add_result(const params_spectr &data);

signals:
    // remove parameter from model
    void signal_remove_from_model(const QString &id_params);
    // send params from model
    void signal_params_from_model(const params_spectr &data);

public slots:
    void slot_set_vector_result(const QVector<params_spectr> &data);
    void slot_remove_from_model(const QString &id_params);

private:
    QVector<params_spectr> m_data;
};

#endif // PARAMS_SPECTR_MODEL_H
