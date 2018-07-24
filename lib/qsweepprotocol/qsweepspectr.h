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
    qreal m_fft_bin_width = 0;      // FFT bin width (frequency resolution) in Hz\n")
    quint32 m_fft_size = 0;
    quint64 m_frequency_min = 0;    // frequency min Hz
    quint64 m_frequency_max = 0;    // frequency max Hz
    QVector<qreal> m_power;
    PowerSpectr() {}
};

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

    void setPowerSpectr(const QVector<PowerSpectr> &);
    QVector<PowerSpectr> powerSpectr()const;

    QByteArray exportToJson(const bool binary = false) const;

private:
    QSharedDataPointer<QSweepSpectrData> data;
};

Q_DECLARE_METATYPE(QSweepSpectr)

#endif // QSWEEPSPECTR_H
