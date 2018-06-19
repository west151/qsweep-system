#include "userinterface.h"

#include "qsweeprequest.h"
#include "qsweepparams.h"

UserInterface::UserInterface(QObject *parent) : QObject(parent),
    m_hostname("127.0.0.1"),
    m_port(1883),
    isConnectedToHost(false)
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

void UserInterface::setConnectedToHost(const bool &value)
{
    isConnectedToHost = value;

     emit connectedToHostChanged();
}

bool UserInterface::connectedToHost() const
{
    return isConnectedToHost;
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
    params.setFrequencyMin(2300);
    params.setFrequencyMax(2700);

    info.setDataRequest(params.exportToJson());
    info.setTypeRequest(TypeRequest::SWEEP_SPECTR);

    emit sendRequestSweepServer(info);
}
