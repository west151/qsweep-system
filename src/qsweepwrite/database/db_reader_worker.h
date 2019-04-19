#ifndef DB_READER_WORKER_H
#define DB_READER_WORKER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "sweep_write_settings.h"
#include "db_state_workers.h"

class db_reader_worker : public QObject
{
    Q_OBJECT
public:
    explicit db_reader_worker(QObject *parent = nullptr);

    void set_configuration(const sweep_write_settings &);

public slots:
    void slot_initialization();
    void slot_launching();
    void slot_stopping();

signals:
    void signal_update_state_workers(const state_workers &type);

private:
    QSqlDatabase m_dbase;
    QString m_str_error_dbase;

    sweep_write_settings m_settings;
};

#endif // DB_READER_WORKER_H
