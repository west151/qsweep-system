#include "user_interface.h"

#include <QVector>

#include "sweep_message.h"
#include "params_spectr.h"
#include "broker_ctrl.h"
#include "sweep_topic.h"

user_interface::user_interface(QObject *parent) : QObject(parent),
    m_freqMin(2300),
    m_freqMax(2700),
    m_lnaGain(32),
    m_vgaGain(30),
    m_fftBinWidth(100000),
    m_oneShot(true),
    m_pingReceivedCount(0)
{

}

void user_interface::onSweepClientSettings(const client_settings &value)
{
    m_client_settings = value;
}

client_settings user_interface::sweepClientSettings() const
{
    return m_client_settings;
}

void user_interface::onPingReceived()
{
    m_pingReceivedCount = m_pingReceivedCount + 1;

    emit pingReceivedCountChanged();
}

quint32 user_interface::pingReceivedCount() const
{
    return m_pingReceivedCount;
}

void user_interface::setHostBroker(const QString &value)
{
    m_client_settings.set_host_broker(value);

    emit hostBrokerChanged();
}

QString user_interface::hostBroker() const
{
    return m_client_settings.host_broker();
}

void user_interface::setPortBroker(const quint16 &value)
{
    m_client_settings.set_port_broker(value);

    emit portBrokerChanged();
}

quint16 user_interface::portBroker() const
{
    return m_client_settings.port_broker();
}

void user_interface::setFrequencyMin(const quint32 &value)
{
    m_freqMin = value;

    emit sendFrequencyMinChanged();
}

quint32 user_interface::frequencyMin() const
{
    return m_freqMin;
}

void user_interface::setFrequencyMax(const quint32 &value)
{
    m_freqMax = value;

    emit sendFrequencyMaxChanged();
}

quint32 user_interface::frequencyMax() const
{
    return m_freqMax;
}

void user_interface::setLnaGain(const quint32 &value)
{
    m_lnaGain = value;

    emit sendLnaGainChanged();
}

quint32 user_interface::lnaGain() const
{
    return m_lnaGain;
}

void user_interface::setVgaGain(const quint32 &value)
{
    m_vgaGain = value;

    emit sendVgaGainChanged();
}

quint32 user_interface::vgaGain() const
{
    return m_vgaGain;
}

void user_interface::setOneShot(const bool &value)
{
    m_oneShot = value;

    emit sendOneShotChanged();
}

bool user_interface::oneShot() const
{
    return m_oneShot;
}

void user_interface::setFFTBinWidth(const quint32 &value)
{
    m_fftBinWidth = value;

    emit sendFFTBinWidthChanged();
}

quint32 user_interface::fftBinWidth() const
{
    return m_fftBinWidth;
}

void user_interface::onRequestSweepInfo()
{
    sweep_message ctrl_info;
    ctrl_info.set_type(type_message::ctrl_info);
    emit signal_sweep_message(ctrl_info.to_json());
}

void user_interface::onRequestSweepSpectr(const bool &start)
{
    sweep_message ctrl_spectr;
    ctrl_spectr.set_type(type_message::ctrl_spectr);

    params_spectr params_spectr_data;

    params_spectr_data.set_id_params(ctrl_spectr.id_message());
    params_spectr_data.set_frequency_min(m_freqMin);
    params_spectr_data.set_frequency_max(m_freqMax);
    params_spectr_data.set_fft_bin_width(m_fftBinWidth);
    params_spectr_data.set_lna_gain(m_lnaGain);
    params_spectr_data.set_vga_gain(m_vgaGain);
    params_spectr_data.set_one_shot(m_oneShot);
    params_spectr_data.set_start_spectr(start);

    ctrl_spectr.set_data_message(params_spectr_data.to_json());

    emit signal_sweep_message(ctrl_spectr.to_json());
}

void user_interface::onClearMaxPowerSpectr()
{
    emit sendClearMaxPowerSpectr();
}

void user_interface::on_spectr_max_calc(const bool &value)
{
    emit signal_spectr_max_calc(value);
}

void user_interface::on_spectr_db_write(const bool &value)
{
    sweep_message ctrl_msg;
    ctrl_msg.set_type(type_message::ctrl_db);

    broker_ctrl db_ctrl;
    sweep_topic topic;
    QStringList list_topic;
    list_topic.append(topic.sweep_topic_by_type(sweep_topic::topic_power_spectr));
    list_topic.append(topic.sweep_topic_by_type(sweep_topic::topic_ctrl));

    if(value)
        db_ctrl.set_ctrl_type(broker_ctrl_type::subscribe);
    else
        db_ctrl.set_ctrl_type(broker_ctrl_type::unsubscribe);

    db_ctrl.set_topic_list(list_topic);

    ctrl_msg.set_data_message(db_ctrl.to_json());

    emit signal_sweep_message(ctrl_msg.to_json());
}

void user_interface::on_read_params_spectr()
{
    emit signal_read_params_spectr();
}

void user_interface::on_save_params_spectr(const QString &descr)
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

void user_interface::slot_set_params_spectr(const params_spectr &data)
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
