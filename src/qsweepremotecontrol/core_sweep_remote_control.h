#ifndef CORE_SWEEP_REMOTE_CONTROL_H
#define CORE_SWEEP_REMOTE_CONTROL_H

#include <QObject>
#include <QQmlApplicationEngine>

#include "settings/remote_control_settings.h"

class top_model;
class lna_gain_model;
class vga_gain_model;
class fft_width_model;
class ui_model;

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
    ui_model* ptr_ui_model {Q_NULLPTR};
    lna_gain_model* ptr_lna_gain_list {Q_NULLPTR};
    vga_gain_model* ptr_vga_gain_list {Q_NULLPTR};
    fft_width_model* ptr_fft_width_model {Q_NULLPTR};

    bool read_settings();
    bool write_settings();
};

#endif // CORE_SWEEP_REMOTE_CONTROL_H
