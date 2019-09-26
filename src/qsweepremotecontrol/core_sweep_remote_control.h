#ifndef CORE_SWEEP_REMOTE_CONTROL_H
#define CORE_SWEEP_REMOTE_CONTROL_H

#include <QObject>
#include <QQmlApplicationEngine>

#include "settings/remote_control_settings.h"

class top_model;

class core_sweep_remote_control : public QObject
{
    Q_OBJECT
public:
    explicit core_sweep_remote_control(const QString &app_name, QObject *parent = nullptr);

    // read file settings
    bool initialization();
    // program launch
    void program_launch(bool is_init_state);

signals:

public slots:

private:
    QString str_config_location;
    remote_control_settings* ptr_remote_control_settings {Q_NULLPTR};

    QQmlApplicationEngine* ptr_engine {Q_NULLPTR};

    // models
    top_model* ptr_top_model {Q_NULLPTR};

    bool read_settings();
    bool write_settings();
};

#endif // CORE_SWEEP_REMOTE_CONTROL_H
