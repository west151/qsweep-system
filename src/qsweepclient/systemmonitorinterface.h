#ifndef SYSTEMMONITORINTERFACE_H
#define SYSTEMMONITORINTERFACE_H

#include <QObject>
#include "qsweepsystemmonitor.h"

class SystemMonitorInterface : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString hostName READ hostName NOTIFY sendSystemMonitorChanged )
    Q_PROPERTY(QString uptime READ uptime NOTIFY sendSystemMonitorChanged)
    Q_PROPERTY(int totalMemory READ totalMemory NOTIFY sendSystemMonitorChanged)
    Q_PROPERTY(int freeMemory READ freeMemory NOTIFY sendSystemMonitorChanged)
    Q_PROPERTY(int bufferMemory READ bufferMemory NOTIFY sendSystemMonitorChanged)

public:
    explicit SystemMonitorInterface(QObject *parent = nullptr);

    QString hostName()const;
    QString uptime()const;
    int totalMemory()const;
    int freeMemory()const;
    int bufferMemory()const;

signals:
    void sendSystemMonitorChanged();

public slots:
    void setSystemMonitor(const QSweepSystemMonitor &);

private:
    QSweepSystemMonitor m_systemMonitor;
};

#endif // SYSTEMMONITORINTERFACE_H
