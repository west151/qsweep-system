#include "sweepparams.h"

class SweepParamsData : public QSharedData
{
public:
    SweepParamsData() :
        QSharedData() {
        m_frequency_min = 1;
        m_frequency_max = 6000;
        m_fft_bin_width = 500000;
        m_lna_gain = 0;
        m_vga_gain = 0;
    }

    SweepParamsData(const SweepParamsData &other) :
        QSharedData(other) {
        m_frequency_min = other.m_frequency_min;
        m_frequency_max = other.m_frequency_max;
        m_fft_bin_width = other.m_fft_bin_width;
        m_lna_gain = other.m_lna_gain;
        m_vga_gain = other.m_vga_gain;
    }

    unsigned int m_lna_gain;    // RX LNA (IF) gain, 0-40dB, 8dB steps
    unsigned int m_vga_gain;    // RX VGA (baseband) gain, 0-62dB, 2dB steps
    uint32_t m_fft_bin_width;   // FFT bin width (frequency resolution) in Hz\n")
    uint32_t m_frequency_min;   // frequency min MHz
    uint32_t m_frequency_max;   // frequency max MHz
};

SweepParams::SweepParams() : data(new SweepParamsData)
{

}

SweepParams::SweepParams(const SweepParams &rhs) : data(rhs.data)
{

}

SweepParams &SweepParams::operator=(const SweepParams &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

SweepParams::~SweepParams()
{

}

void SweepParams::setLnaGain(const unsigned int &value)
{
    data->m_lna_gain = value;
}

unsigned int SweepParams::lnaGain() const
{
    return data->m_lna_gain;
}

void SweepParams::setVgaGain(const unsigned int &value)
{
    data->m_vga_gain = value;
}

unsigned int SweepParams::vgaGain() const
{
    return data->m_vga_gain;
}

void SweepParams::setFFTBinWidth(const uint32_t &value)
{
    data->m_fft_bin_width = value;
}

uint32_t SweepParams::fftBinWidth() const
{
    return data->m_fft_bin_width;
}

void SweepParams::setFrequencyMin(const uint32_t &value)
{
    data->m_frequency_min = value;
}

uint32_t SweepParams::frequencyMin() const
{
    return data->m_frequency_min;
}

void SweepParams::setFrequencyMax(const uint32_t &value)
{
    data->m_frequency_max = value;
}

uint32_t SweepParams::frequencyMax() const
{
    return data->m_frequency_max;
}
