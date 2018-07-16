#ifndef QSWEEPSPECTR_H
#define QSWEEPSPECTR_H

#include <QtCore/qshareddata.h>
#include <QtCore/qmetatype.h>
#include <QDateTime>
#include <QVector>

#include "qsweepprotocol_global.h"

struct PowerSpectr
{
    QDateTime dateTime;
    quint32 m_fft_bin_width;   // FFT bin width (frequency resolution) in Hz\n")
    quint64 m_frequency_min;   // frequency min Hz
    quint64 m_frequency_max;   // frequency max Hz
    QVector<qreal> m_power;
    PowerSpectr() {}
};

//printf("%s, %" PRIu64 ", %" PRIu64 ", %u",
//       time_str,
//       (uint64_t)(frequency),
//       (uint64_t)(frequency+DEFAULT_SAMPLE_RATE_HZ/4),
//       fftSize);

//quint32 m_fft_bin_width;   // FFT bin width (frequency resolution) in Hz\n")
//quint32 m_frequency_min;   // frequency min MHz
//quint32 m_frequency_max;   // frequency max MHz

class QSweepSpectrData;

class QSWEEPPROTOCOLSHARED_EXPORT QSweepSpectr
{
public:
    QSweepSpectr();
    QSweepSpectr(const QSweepSpectr &);
    QSweepSpectr(const QByteArray &json, const bool binary = false);
    QSweepSpectr &operator=(const QSweepSpectr &);
    ~QSweepSpectr();

    bool isValid() const;

    QByteArray exportToJson(const bool binary = false) const;

private:
    QSharedDataPointer<QSweepSpectrData> data;
};

Q_DECLARE_METATYPE(QSweepSpectr)

#endif // QSWEEPSPECTR_H
