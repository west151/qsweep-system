#include "statesweepclient.h"

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

StateSweepClient::StateSweepClient(QObject *parent) : QObject(parent),
    isConnect(false)
{
    ptrMachine = new QStateMachine(this);

    ptrStateConnect = new QState();
    ptrStateDisconnect = new QState();

    ptrMachine->addState(ptrStateConnect);
    ptrMachine->addState(ptrStateDisconnect);

    ptrStateConnect->addTransition(this, &StateSweepClient::onDisconnect, ptrStateDisconnect);
    ptrStateDisconnect->addTransition(this, &StateSweepClient::onConnect, ptrStateConnect);

    ptrStateConnect->assignProperty(this, "stateConnectToBroker", true);
    ptrStateDisconnect->assignProperty(this, "stateConnectToBroker", false);

    ptrMachine->setInitialState(ptrStateDisconnect);

    ptrMachine->start();
}

bool StateSweepClient::stateConnectToBroker() const
{
    return isConnect;
}

void StateSweepClient::setStateConnectToBroker(const bool &on)
{
    isConnect = on;

    emit sendStateConnectToBroker();
}
