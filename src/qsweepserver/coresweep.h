#ifndef CORESWEEP_H
#define CORESWEEP_H

#include <QObject>
#include <QPointer>
#include <QtMqtt/QMqttClient>
#include <QtMqtt/QMqttMessage>
#include <QtMqtt/QMqttSubscription>

#include "settings/sweepserversettings.h"
#include "systemmonitorworker.h"

class HackrfInfo;
class SweepWorker;
class QSweepTopic;
class QSweepAnswer;
class QTimer;

class CoreSweep : public QObject
{
    Q_OBJECT
public:
    explicit CoreSweep(const QString &, QObject *parent = nullptr);

    void onDataFromWorker(const QByteArray &value);

signals:
    void sendRunSweepInfo(const QByteArray &);
    void sendRunSweepWorker(const QByteArray &);
    void sendStopSweepWorker();

private:
    HackrfInfo* ptrHackrfInfo {Q_NULLPTR};
    SweepWorker* ptrSweepWorker {Q_NULLPTR};
    QPointer<QThread> ptrSweepThread;

    QMqttClient* ptrMqttClient {Q_NULLPTR};
    QSweepTopic* ptrSweepTopic {Q_NULLPTR};
    SweepServerSettings* ptrSweepServerSettings {Q_NULLPTR};

    QTimer* ptrTimer {Q_NULLPTR};
    SystemMonitorWorker* ptrSystemMonitorWorker {Q_NULLPTR};
    QPointer<QThread> ptrSystemMonitorThread;

    bool readSettings(const QString &);
    bool saveSettings(const QString &);
    void initialization();
    void launching();

    void messageReceived(const QByteArray &message, const QMqttTopicName &topic = QMqttTopicName());
    void updateLogStateChange();
    void brokerDisconnected();
    void pingReceived();
    void connecting();
    void sendingMessage(const QSweepAnswer &);
    void onSendingMessageRequest(const QByteArray &);
    void errorChanged(QMqttClient::ClientError error);
};

#endif // CORESWEEP_H
