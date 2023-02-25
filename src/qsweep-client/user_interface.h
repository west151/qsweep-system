#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <QObject>
#include "settings/client_settings.h"

class params_spectr;

class user_interface : public QObject
{
    Q_OBJECT
    // spectr params
    Q_PROPERTY(quint32 frequencyMin READ frequencyMin WRITE setFrequencyMin NOTIFY sendFrequencyMinChanged)
    Q_PROPERTY(quint32 frequencyMax READ frequencyMax WRITE setFrequencyMax NOTIFY sendFrequencyMaxChanged)
    Q_PROPERTY(quint32 lnaGain READ lnaGain WRITE setLnaGain NOTIFY sendLnaGainChanged)
    Q_PROPERTY(quint32 vgaGain READ vgaGain WRITE setVgaGain NOTIFY sendVgaGainChanged)
    Q_PROPERTY(quint32 fftBinWidth READ fftBinWidth WRITE setFFTBinWidth NOTIFY sendFFTBinWidthChanged)
    Q_PROPERTY(bool oneShot READ oneShot WRITE setOneShot NOTIFY sendOneShotChanged)
    // mqtt broker params
    Q_PROPERTY(QString hostBroker READ hostBroker WRITE setHostBroker NOTIFY hostBrokerChanged)
    Q_PROPERTY(quint16 portBroker READ portBroker WRITE setPortBroker NOTIFY portBrokerChanged)
    Q_PROPERTY(quint32 pingReceivedCount READ pingReceivedCount NOTIFY pingReceivedCountChanged)

public:
    explicit user_interface(QObject *parent = nullptr);

    // mqtt broker params
    void onSweepClientSettings(const client_settings &);
    client_settings sweepClientSettings()const;
    void onPingReceived();
    quint32 pingReceivedCount()const;

    void setHostBroker(const QString &);
    QString hostBroker()const;
    void setPortBroker(const quint16 &);
    quint16 portBroker()const;

    // spectr params
    void setFrequencyMin(const quint32 &);
    quint32 frequencyMin()const;
    void setFrequencyMax(const quint32 &);
    quint32 frequencyMax()const;
    void setLnaGain(const quint32 &);
    quint32 lnaGain()const;
    void setVgaGain(const quint32 &);
    quint32 vgaGain()const;
    void setOneShot(const bool &);
    bool oneShot()const;
    void setFFTBinWidth(const quint32 &);
    quint32 fftBinWidth()const;

    Q_INVOKABLE void onRequestSweepInfo();
    Q_INVOKABLE void onRequestSweepSpectr(const bool &start = true);
    Q_INVOKABLE void onClearMaxPowerSpectr();
    Q_INVOKABLE void on_spectr_max_calc(const bool &);
    Q_INVOKABLE void on_spectr_db_write(const bool &);
    Q_INVOKABLE void on_read_params_spectr();
    Q_INVOKABLE void on_save_params_spectr(const QString &);

public slots:
    void slot_set_params_spectr(const params_spectr &data);

signals:
    void sendMessageToHost();
    void signal_sweep_message(const QByteArray &);

    // spectr params
    void sendFrequencyMinChanged();
    void sendFrequencyMaxChanged();
    void sendLnaGainChanged();
    void sendVgaGainChanged();
    void sendOneShotChanged();
    void sendFFTBinWidthChanged();
    // mqtt broker params
    void hostBrokerChanged();
    void portBrokerChanged();
    void pingReceivedCountChanged();
    void sendClearMaxPowerSpectr();

    void signal_spectr_max_calc(const bool &);
    void signal_read_params_spectr();
    void signal_save_params_spectr(const QVector<params_spectr> &);

private:
    // spectr params
    quint32 m_freqMin;
    quint32 m_freqMax;
    quint32 m_lnaGain;
    quint32 m_vgaGain;
    quint32 m_fftBinWidth;
    bool m_oneShot;

    // mqtt broker params
    client_settings m_client_settings;
    quint32 m_pingReceivedCount;
};

#endif // USER_INTERFACE_H

