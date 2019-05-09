#ifndef FILE_BACKUP_WORKERS_H
#define FILE_BACKUP_WORKERS_H

#include <QObject>

#include "sweep_write_settings.h"
#include "database/db_state_workers.h"

class file_backup_workers : public QObject
{
    Q_OBJECT
public:
    explicit file_backup_workers(QObject *parent = nullptr);

    void set_configuration(const sweep_write_settings &);

public slots:
    void slot_initialization();
    void slot_launching();
    void slot_stopping();

signals:
    void signal_update_state_workers(const state_workers &);

public slots:

private:
    sweep_write_settings m_settings;
};

#endif // FILE_BACKUP_WORKERS_H
