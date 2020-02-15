#include "db_cleaner_workers.h"
#include "database/db_const.h"

#include <QFileInfo>
#include <QTimer>
#include <QTime>

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

db_cleaner_workers::db_cleaner_workers(QObject *parent) : db_custom_workers(parent)
{
    setObjectName(this->metaObject()->className());

    m_dbase = QSqlDatabase::addDatabase(database_driver, connection_delete);
}

void db_cleaner_workers::slot_clean_db(const QString &file_name)
{
    QFileInfo file_db(file_name);

    if(file_db.exists())
    {
        open_db(file_name);

        if(is_open_db())
        {
            const auto list_table = table.keys();

            QTimer start_time;
            start_time.start();

#ifdef QT_DEBUG
            qDebug() << "start clean database:" << QTime::currentTime().toString();
            qDebug() << "database:" << file_name;
#endif

            for(int i=0; i<list_table.size(); ++i)
            {
                if(is_table_name_resolve(list_table.at(i)))
                {
#ifdef QT_DEBUG
                    qDebug() << "delete from table:" << list_table.at(i);
#endif
                    QSqlQuery* query_delete = new QSqlQuery(m_dbase);
                    //query_delete->prepare(delete_table_sql(list_table.at(i)));

                    if(query_delete->exec(delete_table_sql(list_table.at(i))))
                        update_size_file(m_dbase.databaseName());
                    else
                        update_last_error(query_delete);
                }
            }

            close_db();

            // VACUUM
            open_db(file_name);

            if(is_open_db())
            {
                // VACUUM
                QSqlQuery* query_vacuum = new QSqlQuery(m_dbase);
                query_vacuum->prepare("VACUUM");

                if(query_vacuum->exec())
                    update_size_file(m_dbase.databaseName());
                else
                    update_last_error(query_vacuum);

                close_db();
            }

#ifdef QT_DEBUG
            qDebug() << "stop clean database:" << QTime::currentTime().toString()
                     << QString("Time elapsed: %1 ms").arg(start_time.interval());
#endif

            emit signal_state_db(file_name, state_db::file_is_ready);
        }
    }
}
