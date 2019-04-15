#ifndef DB_PATH_MONITOR_H
#define DB_PATH_MONITOR_H

#include <QObject>

#include "sweep_write_settings.h"
#include "db_state_workers.h"

class db_path_monitor : public QObject
{
    Q_OBJECT
public:
    explicit db_path_monitor(QObject *parent = nullptr);

    void set_configuration(const sweep_write_settings &);

public slots:
    void slot_initialization();
    void slot_launching();
    void slot_stopping();

signals:
    void signal_update_state_workers(const state_workers_type &type);

private:
    sweep_write_settings m_settings;
};

#endif // DB_PATH_MONITOR_H
