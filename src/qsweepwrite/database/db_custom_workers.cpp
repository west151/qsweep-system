#include "db_custom_workers.h"

#include <QSqlError>
#include <QSqlRecord>

#include "db_const.h"

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

db_custom_workers::db_custom_workers(QObject *parent) : QObject(parent)
{

}

void db_custom_workers::set_configuration(const sweep_write_settings &settings)
{
    m_settings = settings;
}

void db_custom_workers::set_database(const QString &driver, const QString &write)
{
    m_dbase = QSqlDatabase::addDatabase(driver, write);
}

void db_custom_workers::open_db(const QString &db_name)
{
    m_dbase.setDatabaseName(db_name);

    if(m_dbase.open())
        set_pragma("synchronous", "0");

    if(!m_dbase.isOpen())
    {
#ifdef QT_DEBUG
        qDebug() << "Can't database open:" << db_name;
        qDebug() << "Error:" << m_dbase.lastError();
#endif
    }
}

void db_custom_workers::close_db()
{
    if(m_dbase.isOpen())
        m_dbase.close();
}

bool db_custom_workers::start_transaction()
{
    bool on(false);
    if(m_dbase.isOpen())
        on = m_dbase.transaction();
    return on;
}

bool db_custom_workers::commit_transaction()
{
    bool on(false);
    if(m_dbase.isOpen())
        on = m_dbase.commit();
    return on;
}

void db_custom_workers::update_last_error(QSqlQuery *query)
{
    if(query)
    {
        m_str_error_dbase = query->lastError().text();

#ifdef QT_DEBUG
        qDebug() << tr("error:") << m_str_error_dbase;
        qDebug() << tr("last query:") << query->lastQuery();
#endif

    }
}

bool db_custom_workers::is_table_name_resolve(const QString &table_name)
{
    if(m_dbase.isOpen()&&(!table_name.isEmpty()))
    {
        QSqlQuery* query = new QSqlQuery(m_dbase);
        QString str_sql = QString("SELECT name FROM sqlite_master WHERE type =:table AND name = '%1' ").arg(table_name);
        query->prepare(str_sql);
        query->bindValue(":table", "table");
        if(!query->exec())
            update_last_error(query);
        int field_no = query->record().indexOf("name");

        while (query->next()) {
            QString _name = query->value(field_no).toString();

            // если таблица существует
            if(_name.contains(table_name, Qt::CaseInsensitive))
                return true;
        }
        return false;
    }
    return false;
}

void db_custom_workers::set_pragma(const QString &param, const QString &value)
{
    QString sql_query(pragma_sql(param, value));

    if (m_dbase.isOpen())
    {
        QSqlQuery* query = new QSqlQuery(m_dbase);
        query->prepare(sql_query);

        if(!query->exec())
            update_last_error(query);
    }
}
