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
class QHackrfInfo;
class DataSource;
class SystemMonitorInterface;
class StateSweepClient;
class SweepClientSettings;

QT_CHARTS_USE_NAMESPACE

class CoreSweepClient : public QObject
{
    Q_OBJECT
public:
    explicit CoreSweepClient(QObject *parent = nullptr);
    int runCoreSweepClient(int argc, char *argv[]);

public slots:
    void onConnectToHost(const QString &host, const quint16 &port);
    void onDisconnectFromHost();

signals:
    void sendStartSpectr();
    void sendStateConnected();
    void sendStateDisconnected();

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
};

#endif // CORESWEEPCLIENT_H
