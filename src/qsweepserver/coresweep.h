#ifndef CORESWEEP_H
#define CORESWEEP_H

#include <QObject>
#include <QPointer>
#include <QtMqtt/QMqttClient>
#include <QtMqtt/QMqttMessage>
#include <QtMqtt/QMqttSubscription>

class HackrfInfo;
class SweepWorker;
class QSweepTopic;
class QSweepAnswer;

class CoreSweep : public QObject
{
    Q_OBJECT
public:
    explicit CoreSweep(QObject *parent = nullptr);

    int runCoreSweep(int argc, char *argv[]);

    void onConnectToHost(const QString &host, const quint16 &port);

signals:
    void sendRunSweepInfo(const QByteArray &);
    void sendRunSweepWorker(const QByteArray &);

private:
    HackrfInfo* ptrHackrfInfo {Q_NULLPTR};
    SweepWorker* ptrSweepWorker {Q_NULLPTR};
    QPointer<QThread> ptrSweepThread;

    QMqttClient* ptrMqttClient {Q_NULLPTR};
    QSweepTopic* ptrSweepTopic {Q_NULLPTR};

    void initialization();
    void launching();

    void messageReceived(const QByteArray &message, const QMqttTopicName &topic = QMqttTopicName());
    void updateLogStateChange();
    void brokerDisconnected();
    void pingReceived();
    void connecting();
    void sendingMessage(const QSweepAnswer &);
};

#endif // CORESWEEP_H
