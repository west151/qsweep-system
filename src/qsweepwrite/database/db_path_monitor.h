#ifndef DB_PATH_MONITOR_H
#define DB_PATH_MONITOR_H

#include <QObject>

#include "sweep_write_settings.h"

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
    void signal_is_initialization(const bool &);
    void signal_is_launching(const bool &);
    void signal_is_stopping(const bool &);

private:
    sweep_write_settings m_settings;
};

#endif // DB_PATH_MONITOR_H
