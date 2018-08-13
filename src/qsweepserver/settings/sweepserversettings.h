#ifndef SWEEPSERVERSETTINGS_H
#define SWEEPSERVERSETTINGS_H

#include <QtCore/qshareddata.h>
#include <QtCore/qmetatype.h>

class SweepServerSettingsData;

class SweepServerSettings
{
public:
    SweepServerSettings();
    SweepServerSettings(const SweepServerSettings &);
    SweepServerSettings(const QByteArray &json, const bool binary = false);
    SweepServerSettings &operator=(const SweepServerSettings &);
    ~SweepServerSettings();

    bool isValid() const;

    void setHostBroker(const QString &);
    QString hostBroker()const;

    void setPortBroker(const quint16 &);
    quint16 portBroker()const;

    QByteArray exportToJson(const bool binary = false) const;

private:
    QSharedDataPointer<SweepServerSettingsData> data;
};

Q_DECLARE_METATYPE(SweepServerSettings)

#endif // SWEEPSERVERSETTINGS_H
