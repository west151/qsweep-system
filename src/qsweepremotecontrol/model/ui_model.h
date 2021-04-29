#ifndef UI_MODEL_H
#define UI_MODEL_H

#include <QObject>

#include "params_spectr.h"

class ui_model : public QObject
{
    Q_OBJECT
    // spectr params
    Q_PROPERTY(quint64 frequency_min READ frequency_min WRITE set_frequency_min NOTIFY signal_frequency_min_changed)
    Q_PROPERTY(quint64 frequency_max READ frequency_max WRITE set_frequency_max NOTIFY signal_frequency_max_changed)
    Q_PROPERTY(quint32 lna_gain READ lna_gain WRITE set_lna_gain NOTIFY signal_lna_gain_changed)
    Q_PROPERTY(quint32 vga_gain READ vga_gain WRITE set_vga_gain NOTIFY signal_vga_gain_changed)
    Q_PROPERTY(quint32 fft_bin_width READ fft_bin_width WRITE set_fft_bin_width NOTIFY signal_fft_bin_width_changed)
    Q_PROPERTY(QString last_error_string READ last_error_string NOTIFY signal_last_error_string_changed)

public:
    explicit ui_model(QObject *parent = nullptr);

    // spectr params
    void set_frequency_min(const quint32 &);
    quint32 frequency_min()const;
    void set_frequency_max(const quint32 &);
    quint32 frequency_max()const;
    void set_lna_gain(const quint32 &);
    quint32 lna_gain()const;
    void set_vga_gain(const quint32 &);
    quint32 vga_gain()const;
    void set_fft_bin_width(const quint32 &);
    quint32 fft_bin_width()const;
    QString last_error_string()const;
    void set_frequency_tuning_step(const quint32 &);
    quint32 frequency_tuning_step()const;

signals:
    // spectr params
    void signal_frequency_min_changed();
    void signal_frequency_max_changed();
    void signal_lna_gain_changed();
    void signal_vga_gain_changed();
    void signal_fft_bin_width_changed();
    void signal_last_error_string_changed();

    void signal_params_spectr_message(const QByteArray &, const QString &);

public slots:

private:
    // spectr params
    params_spectr m_params_spectr_data;
    quint64 m_frequency_tuning_step;
    QString m_last_error_string;

    void set_last_error_string(const QString &value);
};

#endif // UI_MODEL_H
