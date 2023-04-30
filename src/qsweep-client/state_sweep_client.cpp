#include "state_sweep_client.h"

state_sweep_client::state_sweep_client(QObject *parent) : QObject(parent),
    is_connect(false)
{
    ptrMachine = new QStateMachine(this);

    ptrStateConnect = new QState();
    ptrStateDisconnect = new QState();

    ptrMachine->addState(ptrStateConnect);
    ptrMachine->addState(ptrStateDisconnect);

    ptrStateConnect->addTransition(this, &state_sweep_client::onDisconnect, ptrStateDisconnect);
    ptrStateDisconnect->addTransition(this, &state_sweep_client::onConnect, ptrStateConnect);

    ptrStateConnect->assignProperty(this, "stateConnectToBroker", true);
    ptrStateDisconnect->assignProperty(this, "stateConnectToBroker", false);

    ptrMachine->setInitialState(ptrStateDisconnect);

    ptrMachine->start();
}

bool state_sweep_client::stateConnectToBroker() const
{
    return is_connect;
}

void state_sweep_client::setStateConnectToBroker(const bool &on)
{
    is_connect = on;

    emit sendStateConnectToBroker();
}
