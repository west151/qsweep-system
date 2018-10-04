#ifndef QSWEEPSYSTEMMONITOR_H
#define QSWEEPSYSTEMMONITOR_H

#include <QtCore/qshareddata.h>
#include <QtCore/qmetatype.h>

class QSweepSystemMonitorData;

class QSweepSystemMonitor
{
public:
    QSweepSystemMonitor();
    QSweepSystemMonitor(const QSweepSystemMonitor &);
    QSweepSystemMonitor(const QByteArray &json, const bool binary = false);
    QSweepSystemMonitor &operator=(const QSweepSystemMonitor &);
    ~QSweepSystemMonitor();

    bool isValid() const;

    QString hostName()const;
    void setHostName(const QString &);

    QString uptime()const;
    void setUptime(const QString &);

    QString currentCpuArchitecture()const;
    void setCurrentCpuArchitecture(const QString &);

    int totalMemory()const;
    void setTotalMemory(const int &);

    int freeMemory()const;
    void setFreeMemory(const int &);

    int bufferMemory()const;
    void setBufferMemory(const int &);

    QDateTime dateTime()const;

    QByteArray exportToJson(const bool binary = false) const;

private:
    QSharedDataPointer<QSweepSystemMonitorData> data;
};

Q_DECLARE_METATYPE(QSweepSystemMonitor)

#endif // QSWEEPSYSTEMMONITOR_H
