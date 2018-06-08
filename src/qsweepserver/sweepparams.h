#ifndef SWEEPPARAMS_H
#define SWEEPPARAMS_H

#include <QSharedDataPointer>

class SweepParamsData;

class SweepParams
{
public:
    SweepParams();
    SweepParams(const SweepParams &);
    SweepParams &operator=(const SweepParams &);
    ~SweepParams();

    // RX LNA (IF) gain, 0-40dB, 8dB steps
    void setLnaGain(const unsigned int &);
    unsigned int lnaGain()const;
    // RX VGA (baseband) gain, 0-62dB, 2dB steps
    void setVgaGain(const unsigned int &);
    unsigned int vgaGain()const;
    // FFT bin width (frequency resolution) in Hz")
    void setFFTBinWidth(const uint32_t &);
    uint32_t fftBinWidth()const;
    // min frequency  MHz
    void setFrequencyMin(const uint32_t &);
    uint32_t frequencyMin()const;
    // max frequency  MHz
    void setFrequencyMax(const uint32_t &);
    uint32_t frequencyMax()const;

private:
    QSharedDataPointer<SweepParamsData> data;
};

#endif // SWEEPPARAMS_H
