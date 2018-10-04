#ifndef QSWEEPPARAMS_H
#define QSWEEPPARAMS_H

#include <QObject>

#include "qsweepprotocol_global.h"

class QSWEEPPROTOCOLSHARED_EXPORT QSweepParams : public QObject
{
    Q_OBJECT
public:
    explicit QSweepParams(QObject *parent = nullptr);
    explicit QSweepParams(const QByteArray &json, bool binary = false);

    void setOneShot(const bool &);
    bool oneShot()const;

    void setLnaGain(const quint32 &);
    quint32 lnaGain()const;

    void setVgaGain(const quint32 &);
    quint32 vgaGain()const;

    void setFFTBinWidth(const quint32 &);
    quint32 fftBinWidth()const;

    void setFrequencyMin(const quint32 &);
    quint32 frequencyMin()const;

    void setFrequencyMax(const quint32 &);
    quint32 frequencyMax()const;

    QByteArray exportToJson(bool binary = false) const;

private:
    bool m_valid;

    bool m_one_shot;
    quint32 m_lna_gain;    // RX LNA (IF) gain, 0-40dB, 8dB steps
    quint32 m_vga_gain;    // RX VGA (baseband) gain, 0-62dB, 2dB steps
    quint32 m_fft_bin_width;   // FFT bin width (frequency resolution) in Hz\n")
    quint32 m_frequency_min;   // frequency min MHz
    quint32 m_frequency_max;   // frequency max MHz
};

#endif // QSWEEPPARAMS_H
