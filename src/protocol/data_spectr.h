#ifndef DATA_SPECTR_H
#define DATA_SPECTR_H

#include <QSharedData>
#include <QMetaType>
#include <QDateTime>
#include <QVector>

struct power_spectr
{
    QDateTime m_date_time;
    qreal m_fft_bin_width = 0;      // FFT bin width (frequency resolution) in Hz")
    quint32 m_fft_size = 0;         // unused !!!!!!!!!!!!!!!!!!!
    quint64 hz_low = 0;    // frequency min Hz
    quint64 hz_high = 0;    // frequency max Hz
    QVector<qreal> m_power;
    power_spectr() {}
};

class data_spectr_data;

class data_spectr
{
public:
    data_spectr();
    data_spectr(const data_spectr &);
    data_spectr(const QByteArray &json, const bool binary = false);
    data_spectr &operator=(const data_spectr &);
    ~data_spectr();

    bool is_valid() const;

    void set_id_params(const QString &);
    QString id_params()const;

    void set_spectr(const QVector<power_spectr> &);
    QVector<power_spectr> spectr()const;

    QByteArray to_json(const bool binary = false) const;

private:
    QSharedDataPointer<data_spectr_data> data;
};

Q_DECLARE_METATYPE(data_spectr)

#endif // DATA_SPECTR_H
