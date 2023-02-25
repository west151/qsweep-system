#ifndef STATESWEEPCLIENT_H
#define STATESWEEPCLIENT_H

#include <QObject>
#include <QStateMachine>

class StateSweepClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool stateConnectToBroker READ stateConnectToBroker WRITE setStateConnectToBroker NOTIFY sendStateConnectToBroker)
public:
    explicit StateSweepClient(QObject *parent = nullptr);

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

    bool isConnect;
};

#endif // STATESWEEPCLIENT_H
