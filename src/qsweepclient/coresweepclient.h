#ifndef CORESWEEPCLIENT_H
#define CORESWEEPCLIENT_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QtMqtt/QMqttClient>
#include <QtMqtt/QMqttMessage>
#include <QtMqtt/QMqttSubscription>
#include <QValueAxis>
#include <QTime>

#include "model/hackrfinfomodel.h"
#include "model/messagelogmodel.h"

class UserInterface;
class QSweepTopic;
class QSweepRequest;
class DataSource;
class SystemMonitorInterface;
class StateSweepClient;
class SweepClientSettings;
class sweep_message;
class sdr_info;
class data_log;
class system_monitor;

QT_CHARTS_USE_NAMESPACE

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

    DataSource* ptrDataSource {Q_NULLPTR};
    QValueAxis* ptrAxisX {Q_NULLPTR};
    QValueAxis* ptrAxisY {Q_NULLPTR};

    SystemMonitorInterface *ptrSystemMonitorInterface {Q_NULLPTR};
    StateSweepClient *ptrStateSweepClient {Q_NULLPTR};
    QTime *m_timerReceive {Q_NULLPTR};
    qint32 m_sizeDatacReceive;

    // settings
    SweepClientSettings* ptrSweepClientSettings {Q_NULLPTR};

    void initialization();
    bool readSettings(const QString &);
    bool saveSettings(const QString &);
    void launching();

    void messageReceived(const QByteArray &message, const QMqttTopicName &topic = QMqttTopicName());
    void updateLogStateChange();
    void brokerDisconnected();
    void pingReceived();
    void connecting();
    void sendingRequest(const QSweepRequest &);

    void errorChanged(QMqttClient::ClientError error);
};

#endif // CORESWEEPCLIENT_H
