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

    void initialization();
    void launching();
    void stopping();

signals:

public slots:

private:
    sweep_write_settings m_settings;
};

#endif // DB_PATH_MONITOR_H
