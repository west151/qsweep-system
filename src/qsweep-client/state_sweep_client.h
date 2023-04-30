#ifndef STATE_SWEEP_CLIENT_H
#define STATE_SWEEP_CLIENT_H

#include <QObject>
#include <QStateMachine>

class state_sweep_client : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool stateConnectToBroker READ stateConnectToBroker WRITE setStateConnectToBroker NOTIFY sendStateConnectToBroker)
public:
    explicit state_sweep_client(QObject *parent = nullptr);

    bool stateConnectToBroker()const;
    void setStateConnectToBroker(const bool &);

signals:
    void onDisconnect();
    void onConnect();

    void sendStateConnectToBroker();

private:
    QStateMachine *ptrMachine;
    QState *ptrStateConnect;
    QState *ptrStateDisconnect;

    QState *ptrStateRun;
    QState *ptrStateStop;

    bool is_connect;
};

#endif // STATE_SWEEP_CLIENT_H
