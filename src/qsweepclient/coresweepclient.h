#ifndef CORESWEEPCLIENT_H
#define CORESWEEPCLIENT_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QtMqtt/QMqttClient>
#include <QtMqtt/QMqttMessage>
#include <QtMqtt/QMqttSubscription>

#include "model/hackrfinfomodel.h"
#include "model/messagelogmodel.h"

class UserInterface;
class QSweepTopic;
class QSweepRequest;
class QHackrfInfo;

class CoreSweepClient : public QObject
{
    Q_OBJECT
public:
    explicit CoreSweepClient(QObject *parent = nullptr);
    int runCoreSweepClient(int argc, char *argv[]);

public slots:
    void onConnectToHost(const QString &host, const quint16 &port);
    void onDisconnectFromHost();

private:
    QQmlApplicationEngine* ptrEngine {Q_NULLPTR};
    UserInterface* ptrUserInterface {Q_NULLPTR};
    QMqttClient* ptrMqttClient {Q_NULLPTR};
    QSweepTopic* ptrSweepTopic {Q_NULLPTR};
    HackrfInfoModel* ptrHackrfInfoModel {Q_NULLPTR};
    MessageLogModel* ptrMessageLogModel {Q_NULLPTR};

    void initialization();
    bool readSettings() const;
    bool launching() const;

    void messageReceived(const QByteArray &message, const QMqttTopicName &topic = QMqttTopicName());
    void updateLogStateChange();
    void brokerDisconnected();
    void pingReceived();
    void connecting();
    void sendingRequest(const QSweepRequest &);
};

#endif // CORESWEEPCLIENT_H
