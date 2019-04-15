#include "db_writer_worker.h"
#include "db_const.h"

#include <QSqlRecord>
#include <QSqlError>

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

db_writer_worker::db_writer_worker(QObject *parent) : QObject(parent)
{
    setObjectName(this->metaObject()->className());

    m_dbase = QSqlDatabase::addDatabase(database_driver, connection_write);
}

void db_writer_worker::set_configuration(const sweep_write_settings &settings)
{
    m_settings = settings;
}

void db_writer_worker::slot_initialization()
{
    QDir dir(m_settings.db_path());

    if (!dir.exists())
        dir.mkpath(".");

    QStringList list_file(list_of_all_chunk_files(m_settings.db_path(), m_settings.db_chunk()));

    if(dir.exists())
    {
        for(int i=0; i<list_file.size(); ++i)
        {
            const QString tmp_db_name = list_file.at(i);
            m_init_db_status.insert(tmp_db_name, false);

            // create or open (test)
            open_db(tmp_db_name);

            m_init_db_status.insert(tmp_db_name, m_dbase.isOpen());

            close_db();
        }

        qDebug() << m_init_db_status.values();

        emit signal_update_state_workers(state_workers_type::initialization);
    }
}

void db_writer_worker::slot_launching()
{
    // to do

    emit signal_update_state_workers(state_workers_type::launching);
}

void db_writer_worker::slot_stopping()
{
    // to do

    emit signal_update_state_workers(state_workers_type::stopping);
}

void db_writer_worker::open_db(const QString &db_name)
{
    m_dbase.setDatabaseName(db_name);

    if(m_dbase.open())
    {
#ifdef QT_DEBUG
        qDebug() << "Open database:" << db_name;
#endif

        //PRAGMA page_size = bytes; // размер страницы БД; страница БД - это единица обмена между диском и кэшом, разумно сделать равным размеру кластера диска (у меня 4096)
        //PRAGMA cache_size = -kibibytes; // задать размер кэша соединения в килобайтах, по умолчанию он равен 2000 страниц БД
        //PRAGMA encoding = "UTF-8";  // тип данных БД, всегда используйте UTF-8
        //PRAGMA foreign_keys = 1; // включить поддержку foreign keys, по умолчанию - ОТКЛЮЧЕНА
        //PRAGMA journal_mode = DELETE | TRUNCATE | PERSIST | MEMORY | WAL | OFF;  // задать тип журнала, см. далее
        //PRAGMA synchronous = 0 | OFF | 1 | NORMAL | 2 | FULL; // тип синхронизации транзакции, см. далее

        set_pragma("synchronous", "0");

    }else{
#ifdef QT_DEBUG
        qDebug() << "Can't database open:" << db_name;
        qDebug() << "Error:" << m_dbase.lastError();
#endif
    }


//    m_dbase.setDatabaseName(full_path);

//    if(m_dbase.open())
//    {
//#ifdef QT_DEBUG
//        qDebug() << Q_FUNC_INFO << "Open database:" << full_path;
//#endif
//        const auto list_table = m_table.keys();

//        for(int i=0; i<list_table.size(); i++)
//            if(!is_table_name_resolve(list_table.at(i)))
//                create_table(list_table.at(i));



//        set_pragma("synchronous","0");

////#ifdef QT_DEBUG
////        // for test
////        QVector<params_spectr> tmpVector;
////        params_spectr data;
////        data.set_frequency_min(2200000);
////        data.set_frequency_max(2700000);
////        data.set_lna_gain(32);
////        data.set_vga_gain(20);
////        data.set_descr(tr("describe"));

////        tmpVector.append(data);
////        slot_write_params_spectr(tmpVector);
////#endif

//    }else{
//#ifdef QT_DEBUG
//        qDebug() << Q_FUNC_INFO << "Can't database open:" << full_path;
//        qDebug() << Q_FUNC_INFO << "error:" << m_dbase.lastError();
//#endif
    //    }
}

bool db_writer_worker::start_transaction()
{
    bool on(false);
    if(m_dbase.isOpen())
        on = m_dbase.transaction();
    return on;
}

bool db_writer_worker::commit_transaction()
{
    bool on(false);
    if(m_dbase.isOpen())
        on = m_dbase.commit();
    return on;
}

void db_writer_worker::set_pragma(const QString &param, const QString &value)
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

bool db_writer_worker::is_table_name_resolve(const QString &table_name)
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

void db_writer_worker::update_last_error(QSqlQuery *query)
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

void db_writer_worker::close_db()
{
    if(m_dbase.isOpen())
        m_dbase.close();
}
