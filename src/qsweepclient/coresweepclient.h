#ifndef CORESWEEPCLIENT_H
#define CORESWEEPCLIENT_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QtMqtt/QMqttClient>
#include <QtMqtt/QMqttMessage>
#include <QtMqtt/QMqttSubscription>
#include <QTime>
#include <QPointer>

#include "model/hackrfinfomodel.h"
#include "model/messagelogmodel.h"

class UserInterface;
class QSweepTopic;
class SystemMonitorInterface;
class StateSweepClient;
class SweepClientSettings;
class sweep_message;
class sdr_info;
class data_log;
class system_monitor;
class ta_spectr;
class data_spectr;

class CoreSweepClient : public QObject
{
    Q_OBJECT
public:
    explicit CoreSweepClient(QObject *parent = nullptr);
    int runCoreSweepClient(int argc, char *argv[]);

public slots:
    void onDisconnectFromHost();

signals:
    void sendStartSpectr();
    void sendStateConnected();
    void sendStateDisconnected();

    void signal_sdr_info(const sdr_info &);
    void signal_data_log(const data_log &);
    void signal_system_monitor(const system_monitor &);
    void signal_data_spectr(const data_spectr &);

private slots:
    void slot_publish_message(const QByteArray &);
    void slot_message_received(const QByteArray &message, const QMqttTopicName &topic = QMqttTopicName());

private:
    QQmlApplicationEngine* ptrEngine {Q_NULLPTR};
    UserInterface* ptrUserInterface {Q_NULLPTR};
    QMqttClient* ptrMqttClient {Q_NULLPTR};
    QSweepTopic* ptrSweepTopic {Q_NULLPTR};
    HackrfInfoModel* ptrHackrfInfoModel {Q_NULLPTR};
    MessageLogModel* ptrMessageLogModel {Q_NULLPTR};

    SystemMonitorInterface *ptrSystemMonitorInterface {Q_NULLPTR};
    StateSweepClient *ptrStateSweepClient {Q_NULLPTR};
    QTime *m_timerReceive {Q_NULLPTR};
    qint32 m_sizeDatacReceive;

    // settings
    SweepClientSettings* ptrSweepClientSettings {Q_NULLPTR};

    // ta spectr
    ta_spectr* ptr_ta_spectr_worker {Q_NULLPTR};
    QPointer<QThread> ptr_ta_spectr_thread;

    void initialization();
    bool readSettings(const QString &);
    bool saveSettings(const QString &);
    void launching();

    void updateLogStateChange();
    void brokerDisconnected();
    void pingReceived();
    void connecting();

    void errorChanged(QMqttClient::ClientError error);
};

#endif // CORESWEEPCLIENT_H
