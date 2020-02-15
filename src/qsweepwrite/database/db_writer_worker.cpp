#include "db_writer_worker.h"
#include "db_const.h"

#include <QSqlRecord>
#include <QSqlError>
#include <QFileInfo>
#include <QTimer>

#include "sweep_message.h"

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

    if(dir.exists())
    {
        QStringList list_file(list_of_all_chunk_files(m_settings.db_path(), m_settings.db_file_count()));

        for(int i=0; i<list_file.size(); ++i)
        {
            const QString tmp_db_name = list_file.at(i);
            m_init_db_file_status.insert(tmp_db_name, false);

            m_db_file_state.insert(tmp_db_name, state_db::file_unknown);
            emit signal_state_db(tmp_db_name, state_db::file_unknown);

            // create or open (test)
            open_db(tmp_db_name);

            // clear data
            //clean_file_db(tmp_db_name);

            update_size_file(tmp_db_name);

            m_init_db_file_status.insert(tmp_db_name, m_dbase.isOpen());

            if(m_dbase.isOpen())
                close_db();
        }

        const auto is_state_list = m_init_db_file_status.values();

        if(!is_state_list.contains(false))
            emit signal_update_state_workers(state_workers::initialization);

    }else {
#ifdef QT_DEBUG
        qCritical() << "Can't find path:" << dir.absolutePath();
#endif
    }
}

void db_writer_worker::slot_launching()
{
    close_db();

    open_db(file_selection_for_writing());

    if(m_dbase.isOpen())
        emit signal_update_state_workers(state_workers::launching);
}

void db_writer_worker::slot_stopping()
{
    // to do

    emit signal_update_state_workers(state_workers::stopping);
}

void db_writer_worker::slot_data_to_write(const QByteArray &rc_data)
{
    const sweep_message data_received(rc_data);

    if(data_received.is_valid())
    {
        if(data_received.type() == type_message::data_spectr)
        {
            const data_spectr rc_data_spectr(data_received.data_message());
            data_spectr_to_write(rc_data_spectr);
        }

        if(data_received.type() == type_message::ctrl_spectr)
        {
            const params_spectr rc_params_spectr_data(data_received.data_message());
            data_params_to_write(rc_params_spectr_data);
        }
    }
}

void db_writer_worker::slot_file_is_ready(const QString &db_name)
{
    if(m_db_file_state.value(db_name)!=state_db::file_is_ready)
    {

        m_db_file_state.insert(db_name, state_db::file_is_ready);

#ifdef QT_DEBUG
        qInfo() << tr("----------------------------------------");
        qInfo() << tr("file is ready:") << db_name;
        qInfo() << tr("files update state:") << m_db_file_state.values();
        qInfo() << tr("----------------------------------------");
#endif
    }
}

void db_writer_worker::open_db(const QString &db_name)
{
    m_dbase.setDatabaseName(db_name);

    if(m_dbase.open())
    {
        //PRAGMA page_size = bytes; // размер страницы БД; страница БД - это единица обмена между диском и кэшом, разумно сделать равным размеру кластера диска (у меня 4096)
        //PRAGMA cache_size = -kibibytes; // задать размер кэша соединения в килобайтах, по умолчанию он равен 2000 страниц БД
        //PRAGMA encoding = "UTF-8";  // тип данных БД, всегда используйте UTF-8
        //PRAGMA foreign_keys = 1; // включить поддержку foreign keys, по умолчанию - ОТКЛЮЧЕНА
        //PRAGMA journal_mode = DELETE | TRUNCATE | PERSIST | MEMORY | WAL | OFF;  // задать тип журнала, см. далее
        //PRAGMA synchronous = 0 | OFF | 1 | NORMAL | 2 | FULL; // тип синхронизации транзакции, см. далее
        //PRAGMA schema.auto_vacuum = 0 | NONE | 1 | FULL | 2 | INCREMENTAL;

        set_pragma("synchronous", "0");
        set_pragma("auto_vacuum", "1");

        const auto list_table = table.keys();

        for(int i=0; i<list_table.size(); i++)
            if(!is_table_name_resolve(list_table.at(i)))
                create_table(list_table.at(i));

    }else{
#ifdef QT_DEBUG
        qDebug() << "Can't database open:" << db_name;
        qDebug() << "Error:" << m_dbase.lastError();
#endif
    }
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

bool db_writer_worker::create_table(const QString &table_name) const
{
    if(m_dbase.isOpen()&&(!table_name.isEmpty()))
    {
        QSqlQuery query(m_dbase);
        QString sql = create_table_sql(table_name);

        if(query.exec(sql)){
#ifdef QT_DEBUG
            qDebug() << "create table:" << table_name;
#endif
            return true;
        } else {
#ifdef QT_DEBUG
            qDebug() << "can't create table:" << table_name;
            qDebug() << tr("error:") << query.lastError().text();
            qDebug() << tr("last query:") << query.lastQuery();
#endif
            return false;
        }
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

void db_writer_worker::data_spectr_to_write(const data_spectr &data)
{
    if(m_dbase.isOpen()&&(m_db_file_state.value(m_dbase.databaseName()) == state_db::file_is_ready))
    {
        QSqlQuery* query = new QSqlQuery(m_dbase);
        query->prepare(insert_table_sql(spectr_data_table));

        QByteArray ba(data.to_json());
        query->bindValue(":params_id", data.id_params());
        query->bindValue(":data_spectr", ba);

        bool on = query->exec();

        if(on)
        {
            //qDebug() << "id_params:" << data.id_params(); // << data.to_json();
            update_size_file(m_dbase.databaseName());
        }

        if(!on)
            update_last_error(query);
    }


    if(m_db_file_state.value(m_dbase.databaseName()) == state_db::file_is_full)
    {
        qDebug() << "file is full:" << m_dbase.databaseName();

        close_db();

        QString db_file = file_selection_for_writing();

        if(!db_file.isEmpty())
        {            
            open_db(file_selection_for_writing());
        } else {
            qDebug() << "all files are full:" << m_dbase.databaseName();
        }
    }
}

void db_writer_worker::data_params_to_write(const params_spectr &data_params)
{
    if(m_dbase.isOpen()&&(m_db_file_state.value(m_dbase.databaseName()) == state_db::file_is_ready))
    {
        QSqlQuery* query = new QSqlQuery(m_dbase);
        query->prepare(insert_table_sql(spectr_params_table));

        QByteArray ba(data_params.to_json());
        query->bindValue(":params_id", data_params.id_params());
        query->bindValue(":data_params", ba);

        bool on = query->exec();

        if(on)
            update_size_file(m_dbase.databaseName());

        if(!on)
            update_last_error(query);
    }
}

void db_writer_worker::update_size_file(const QString &db_name)
{
    qint64 size = 0;
    QFileInfo info(db_name);

    size = info.size();

    m_db_file_size.insert(db_name, size);

    if(size >= m_settings.db_file_size()*1024*1024)
    {
        m_db_file_state.insert(db_name, state_db::file_is_full);

        if(m_dbase.isOpen())
            close_db();

        emit signal_state_db(db_name, state_db::file_is_full);

    }else{
        m_db_file_state.insert(db_name, state_db::file_is_ready);
        emit signal_state_db(db_name, state_db::file_is_ready);
    }

    emit signal_file_size(db_name, size);
}

QString db_writer_worker::file_selection_for_writing() const
{
    const auto file_list = m_db_file_state.keys();

    for(int i=0; i<file_list.size(); ++i)
        if(m_db_file_state.value(file_list.at(i)) != state_db::file_is_full)
            return file_list.at(i);

    return "";
}

void db_writer_worker::clean_file_db(const QString &file_name)
{
    QFileInfo file_db(file_name);

    if(file_db.exists())
    {
        open_db(file_name);

        if(m_dbase.isOpen())
        {
            const auto list_table = table.keys();

            QTimer start_time;
            start_time.start();

#ifdef QT_DEBUG
            //qDebug() << "start clean database:" << start_time.toString();
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
                    if(!query_delete->exec(delete_table_sql(list_table.at(i))))
                        update_last_error(query_delete);
                }
            }

            close_db();

//#ifdef QT_DEBUG
//            qDebug() << "stop clean database:" << start_time.currentTime().toString()
//                     << QString("Time elapsed: %1 ms").arg(start_time.elapsed());
//#endif
        }
    }
}

void db_writer_worker::close_db()
{
    if(m_dbase.isOpen())
        m_dbase.close();
}
