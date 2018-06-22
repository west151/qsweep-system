#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QObject>

class QSweepRequest;

class UserInterface : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
    Q_PROPERTY(quint16 port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(bool connectedToHost READ connectedToHost NOTIFY connectedToHostChanged)
    // spectr params
    Q_PROPERTY(quint32 frequencyMin READ frequencyMin WRITE setFrequencyMin NOTIFY sendFrequencyMinChanged)
    Q_PROPERTY(quint32 frequencyMax READ frequencyMax WRITE setFrequencyMax NOTIFY sendFrequencyMaxChanged)

public:
    explicit UserInterface(QObject *parent = nullptr);

    void setHost(const QString &);
    QString host() const;

    void setPort(const quint16 &);
    quint16 port() const;

    void setConnectedToHost(const bool &);
    bool connectedToHost()const;

    // spectr params
    void setFrequencyMin(const quint32 &);
    quint32 frequencyMin()const;

    void setFrequencyMax(const quint32 &);
    quint32 frequencyMax()const;

    Q_INVOKABLE void onConnectToHost();
    Q_INVOKABLE void onDisconnectFromHost();
    Q_INVOKABLE void onRequestSweepInfo();
    Q_INVOKABLE void onRequestSweepSpectr();

signals:
    void hostChanged();
    void portChanged();
    void connectedToHostChanged();
    void sendConnectToHost(const QString &host, const quint16 &port);
    void sendMessageToHost();
    void sendDisconnectFromHost();
    void sendRequestSweepServer(const QSweepRequest &);
    // spectr params
    void sendFrequencyMinChanged();
    void sendFrequencyMaxChanged();

private:
    QString m_hostname;
    quint16 m_port;
    bool isConnectedToHost;

    quint32 m_freqMin;
    quint32 m_freqMax;
};

#endif // USERINTERFACE_H
