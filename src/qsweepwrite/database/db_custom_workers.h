#ifndef DB_CUSTOM_WORKERS_H
#define DB_CUSTOM_WORKERS_H

#include <QObject>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "sweep_write_settings.h"

class db_custom_workers : public QObject
{
    Q_OBJECT
public:
    explicit db_custom_workers(QObject *parent = nullptr);

    void set_configuration(const sweep_write_settings &);
    void set_database(const QString &driver, const QString &write);

signals:

public slots:

private:
    QSqlDatabase m_dbase;
    QString m_str_error_dbase;
    sweep_write_settings m_settings;

    void open_db(const QString &);
    void close_db();

    bool start_transaction();
    bool commit_transaction();

    void update_last_error(QSqlQuery* query);

    bool is_table_name_resolve(const QString &);

    void set_pragma(const QString &, const QString &);
};

#endif // DB_CUSTOM_WORKERS_H
