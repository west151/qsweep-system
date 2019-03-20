#include "userinterface.h"

#include <QVector>

#include "sweep_message.h"
#include "params_spectr.h"

UserInterface::UserInterface(QObject *parent) : QObject(parent),
    m_freqMin(2300),
    m_freqMax(2700),
    m_lnaGain(32),
    m_vgaGain(30),
    m_fftBinWidth(100000),
    m_oneShot(true),
    m_pingReceivedCount(0)
{

}

void UserInterface::onSweepClientSettings(const SweepClientSettings &value)
{
    m_sweepClientSettings = value;
}

SweepClientSettings UserInterface::sweepClientSettings() const
{
    return m_sweepClientSettings;
}

void UserInterface::onPingReceived()
{
    m_pingReceivedCount = m_pingReceivedCount + 1;

    emit pingReceivedCountChanged();
}

quint32 UserInterface::pingReceivedCount() const
{
    return m_pingReceivedCount;
}

void UserInterface::setHostBroker(const QString &value)
{
    m_sweepClientSettings.setHostBroker(value);

    emit hostBrokerChanged();
}

QString UserInterface::hostBroker() const
{
    return m_sweepClientSettings.hostBroker();
}

void UserInterface::setPortBroker(const quint16 &value)
{
    m_sweepClientSettings.setPortBroker(value);

    emit portBrokerChanged();
}

quint16 UserInterface::portBroker() const
{
    return m_sweepClientSettings.portBroker();
}

void UserInterface::setFrequencyMin(const quint32 &value)
{
    m_freqMin = value;

    emit sendFrequencyMinChanged();
}

quint32 UserInterface::frequencyMin() const
{
    return m_freqMin;
}

void UserInterface::setFrequencyMax(const quint32 &value)
{
    m_freqMax = value;

    emit sendFrequencyMaxChanged();
}

quint32 UserInterface::frequencyMax() const
{
    return m_freqMax;
}

void UserInterface::setLnaGain(const quint32 &value)
{
    m_lnaGain = value;

    emit sendLnaGainChanged();
}

quint32 UserInterface::lnaGain() const
{
    return m_lnaGain;
}

void UserInterface::setVgaGain(const quint32 &value)
{
    m_vgaGain = value;

    emit sendVgaGainChanged();
}

quint32 UserInterface::vgaGain() const
{
    return m_vgaGain;
}

void UserInterface::setOneShot(const bool &value)
{
    m_oneShot = value;

    emit sendOneShotChanged();
}

bool UserInterface::oneShot() const
{
    return m_oneShot;
}

void UserInterface::setFFTBinWidth(const quint32 &value)
{
    m_fftBinWidth = value;

    emit sendFFTBinWidthChanged();
}

quint32 UserInterface::fftBinWidth() const
{
    return m_fftBinWidth;
}

void UserInterface::onRequestSweepInfo()
{
    sweep_message ctrl_info;
    ctrl_info.set_type(type_message::CTRL_INFO);
    emit signal_sweep_message(ctrl_info.export_json());
}

void UserInterface::onRequestSweepSpectr(const bool &start)
{
    sweep_message ctrl_spectr;
    ctrl_spectr.set_type(type_message::CTRL_SPECTR);

    params_spectr params_spectr_data;

    params_spectr_data.set_frequency_min(m_freqMin);
    params_spectr_data.set_frequency_max(m_freqMax);
    params_spectr_data.set_fft_bin_width(m_fftBinWidth);
    params_spectr_data.set_lna_gain(m_lnaGain);
    params_spectr_data.set_vga_gain(m_vgaGain);
    params_spectr_data.set_one_shot(m_oneShot);
    params_spectr_data.set_start_spectr(start);

    ctrl_spectr.set_data_message(params_spectr_data.export_json());

    emit signal_sweep_message(ctrl_spectr.export_json());
}

void UserInterface::onClearMaxPowerSpectr()
{
    emit sendClearMaxPowerSpectr();
}

void UserInterface::on_spectr_max_calc(const bool &value)
{
    emit signal_spectr_max_calc(value);
}

void UserInterface::on_read_params_spectr()
{
    emit signal_read_params_spectr();
}

void UserInterface::on_save_params_spectr(const QString &descr)
{
    QVector<params_spectr> tmpVector;

    params_spectr params_spectr_data;
    params_spectr_data.set_frequency_min(m_freqMin);
    params_spectr_data.set_frequency_max(m_freqMax);
    params_spectr_data.set_fft_bin_width(m_fftBinWidth);
    params_spectr_data.set_lna_gain(m_lnaGain);
    params_spectr_data.set_vga_gain(m_vgaGain);
    params_spectr_data.set_descr(descr);

    tmpVector.append(params_spectr_data);

    emit signal_save_params_spectr(tmpVector);
}

void UserInterface::slot_set_params_spectr(const params_spectr &data)
{
    //
    onRequestSweepSpectr(false);

    setFrequencyMin(data.frequency_min());
    setFrequencyMax(data.frequency_max());
    setLnaGain(data.lna_gain());
    setVgaGain(data.vga_gain());
    setFFTBinWidth(data.fft_bin_width());

    onRequestSweepSpectr(true);
}
