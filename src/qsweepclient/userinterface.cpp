#include "userinterface.h"

#include "qsweeprequest.h"
#include "qsweepparams.h"

UserInterface::UserInterface(QObject *parent) : QObject(parent),
    m_hostname("127.0.0.1"),
    m_port(1883),
    isConnectedToHost(false),
    m_freqMin(30),
    m_freqMax(6000),
    m_lnaGain(0),
    m_vgaGain(0),
    m_fftBinWidth(500000),
    m_oneShot(true)
{

}

void UserInterface::setHost(const QString &host)
{
    m_hostname.clear();
    m_hostname = host;

    emit hostChanged();
}

QString UserInterface::host() const
{
    return m_hostname;
}

void UserInterface::setPort(const quint16 &port)
{
    m_port = port;

    emit portChanged();
}

quint16 UserInterface::port() const
{
    return m_port;
}

void UserInterface::onSweepClientSettings(const SweepClientSettings &value)
{
    m_sweepClientSettings = value;
}

SweepClientSettings UserInterface::sweepClientSettings() const
{
    return m_sweepClientSettings;
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

void UserInterface::setConnectedToHost(const bool &value)
{
    isConnectedToHost = value;

     emit connectedToHostChanged();
}

bool UserInterface::connectedToHost() const
{
    return isConnectedToHost;
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

void UserInterface::onConnectToHost()
{
    emit sendConnectToHost(m_hostname, m_port);
}

void UserInterface::onDisconnectFromHost()
{
    emit sendDisconnectFromHost();
}

void UserInterface::onRequestSweepInfo()
{
    QSweepRequest info(this);
    info.setTypeRequest(TypeRequest::INFO);

    emit sendRequestSweepServer(info);
}

void UserInterface::onRequestSweepSpectr()
{
    QSweepRequest info(this);
    QSweepParams params(this);
    params.setFrequencyMin(m_freqMin);
    params.setFrequencyMax(m_freqMax);
    params.setFFTBinWidth(m_fftBinWidth);
    params.setLnaGain(m_lnaGain);
    params.setVgaGain(m_vgaGain);

    info.setDataRequest(params.exportToJson());
    info.setTypeRequest(TypeRequest::SWEEP_SPECTR);

    emit sendRequestSweepServer(info);
}
