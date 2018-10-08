#ifndef SWEEPCLIENTSETTINGS_H
#define SWEEPCLIENTSETTINGS_H

#include <QtCore/qshareddata.h>
#include <QtCore/qmetatype.h>

class SweepClientSettingsData;

class SweepClientSettings
{
public:
    SweepClientSettings();
    SweepClientSettings(const SweepClientSettings &);
    SweepClientSettings(const QByteArray &json, const bool binary = false);
    SweepClientSettings &operator=(const SweepClientSettings &);
    ~SweepClientSettings();

    bool isValid() const;

    void setHostBroker(const QString &);
    QString hostBroker()const;

    void setPortBroker(const quint16 &);
    quint16 portBroker()const;

    void setMaxSizeMessageLog(const qint32 &);
    qint32 maxSizeMessageLog()const;

    QByteArray exportToJson(const bool binary = false, const bool isCompact = true) const;

private:
    QSharedDataPointer<SweepClientSettingsData> data;
};

Q_DECLARE_METATYPE(SweepClientSettings)

#endif // SWEEPCLIENTSETTINGS_H
