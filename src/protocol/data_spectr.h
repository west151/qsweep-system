#ifndef DATA_SPECTR_H
#define DATA_SPECTR_H

#include <QSharedData>
#include <QMetaType>
#include <QDateTime>
#include <QVector>

struct power_spectr
{
    QDateTime dateTime;
    qreal m_fft_bin_width = 0;      // FFT bin width (frequency resolution) in Hz")
    quint32 m_fft_size = 0;
    quint64 m_frequency_min = 0;    // frequency min Hz
    quint64 m_frequency_max = 0;    // frequency max Hz
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

    void set_spectr(const QVector<power_spectr> &);
    QVector<power_spectr> spectr()const;

    QByteArray export_json(const bool binary = false) const;

private:
    QSharedDataPointer<data_spectr_data> data;
};

Q_DECLARE_METATYPE(data_spectr)

#endif // DATA_SPECTR_H
