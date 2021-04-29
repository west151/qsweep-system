#ifndef CORE_SWEEP_WRITE_H
#define CORE_SWEEP_WRITE_H

#include <QObject>

#include "sweep_write_settings.h"

class mqtt_provider;
class db_manager;

class core_sweep_write : public QObject
{
    Q_OBJECT
public:
    explicit core_sweep_write(const QString &, QObject *parent = nullptr);

signals:

public slots:

private:
    sweep_write_settings m_sweep_write_settings;
    mqtt_provider *ptr_mqtt_provider {Q_NULLPTR};
    db_manager *ptr_db_manager {Q_NULLPTR};

    void initialization();
    void launching();

    bool read_settings(const QString &);
    bool save_settings(const QString &);
};

#endif // CORE_SWEEP_WRITE_H
