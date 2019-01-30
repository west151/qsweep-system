#include "db_local_state.h"
#include <QDir>

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

static const QString database_driver = "QSQLITE";
static const QString database_name = "db_local_state.sqlite";
static const QString connection_name = "visual_settings";
static const QString visual_settings_table = "visual_settings_tbl";

// SQLITE data type
static const QString sqlite_type_numeric(QString("NUMERIC"));
static const QString sqlite_type_integer(QString("INTEGER"));
static const QString sqlite_type_integer_pk(QString("NTEGER PRIMARY KEY"));
static const QString sqlite_type_char(QString("CHAR(%1)"));
static const QString sqlite_type_varchar(QString("VARCHAR(%1)"));
static const QString sqlite_type_floatnone(QString("FLOATNONE"));
static const QString sqlite_type_datetime(QString("DATETIME"));
static const QString sqlite_type_double(QString("DOUBLE PRECISION"));

db_local_state::db_local_state(QObject *parent) : QObject(parent)
{
    m_dbase = QSqlDatabase::addDatabase(database_driver, connection_name);

    m_column_visual_settings.insert("id_pk", sqlite_type_integer_pk);
    m_column_visual_settings.insert("id_params", sqlite_type_char.arg(8));
    m_column_visual_settings.insert("min_freq", sqlite_type_double);
    m_column_visual_settings.insert("max_freq", sqlite_type_double);
    m_column_visual_settings.insert("lna_gain", sqlite_type_integer);
    m_column_visual_settings.insert("vga_gain", sqlite_type_integer);
    m_column_visual_settings.insert("fft_bin_width", sqlite_type_double);
    m_column_visual_settings.insert("descr", sqlite_type_varchar.arg(250));

    m_table.insert(visual_settings_table, m_column_visual_settings);
}

void db_local_state::slot_open_db(const QString &dbpath)
{
    QDir dir(dbpath);

    if (!dir.exists())
        dir.mkpath(".");

    QString full_path(dbpath);
    full_path.append(QDir::separator());
    full_path.append(database_name);

    m_dbase.setDatabaseName(full_path);

    if(m_dbase.open())
    {
#ifdef QT_DEBUG
        qDebug() << Q_FUNC_INFO << "Open database:" << full_path;
#endif
        const auto list_table = m_table.keys();

        for(int i=0; i<list_table.size(); i++)
            if(!is_table_name_resolve(list_table.at(i)))
                create_table(list_table.at(i));

        //PRAGMA page_size = bytes; // размер страницы БД; страница БД - это единица обмена между диском и кэшом, разумно сделать равным размеру кластера диска (у меня 4096)
        //PRAGMA cache_size = -kibibytes; // задать размер кэша соединения в килобайтах, по умолчанию он равен 2000 страниц БД
        //PRAGMA encoding = "UTF-8";  // тип данных БД, всегда используйте UTF-8
        //PRAGMA foreign_keys = 1; // включить поддержку foreign keys, по умолчанию - ОТКЛЮЧЕНА
        //PRAGMA journal_mode = DELETE | TRUNCATE | PERSIST | MEMORY | WAL | OFF;  // задать тип журнала, см. далее
        //PRAGMA synchronous = 0 | OFF | 1 | NORMAL | 2 | FULL; // тип синхронизации транзакции, см. далее

        set_pragma("synchronous","0");

#ifdef QT_DEBUG
        // for test
        QVector<params_spectr> tmpVector;
        params_spectr data;
        data.set_frequency_min(2200000);
        data.set_frequency_max(2700000);
        data.set_lna_gain(32);
        data.set_vga_gain(20);
        data.set_descr(tr("describe"));

        tmpVector.append(data);
        slot_write_params_spectr(tmpVector);
#endif

    }else{
#ifdef QT_DEBUG
        qDebug() << Q_FUNC_INFO << "Can't database open:" << full_path;
        qDebug() << Q_FUNC_INFO << "error:" << m_dbase.lastError();
#endif
    }
}

void db_local_state::slot_write_params_spectr(const QVector<params_spectr> &value)
{
    if((value.size()>0)&&(m_dbase.isOpen()))
    {
        QStringList str_insert_table;

        str_insert_table << "INSERT INTO"
                         << visual_settings_table
                         << "(" << list_column() << ")"
                         << "VALUES"
                         << "(" << list_column(":") << ");";

        QSqlQuery* query = new QSqlQuery(m_dbase);

        QString str_insert_sql(str_insert_table.join(" "));
        query->prepare(str_insert_sql);

        start_transaction();

        for(int i=0; i<value.size(); ++i)
        {
            query->bindValue(":id_params", value.at(i).id_params_spectr());
            query->bindValue(":min_freq", value.at(i).frequency_min());
            query->bindValue(":max_freq", value.at(i).frequency_max());
            query->bindValue(":lna_gain", value.at(i).lna_gain());
            query->bindValue(":vga_gain", value.at(i).vga_gain());
            query->bindValue(":fft_bin_width", value.at(i).fft_bin_width());
            query->bindValue(":descr", value.at(i).descr());
        }

        bool on = query->exec();

        if(!on){
            m_str_error_dbase = query->lastError().text();
#ifdef QT_DEBUG
            qDebug() << Q_FUNC_INFO << tr("error:") << query->lastError().text();
            qDebug() << Q_FUNC_INFO << tr("last query:") << query->lastQuery();
#endif
        }
        commit_transaction();
    }
}

void db_local_state::slot_sync_params_spectr(const QVector<params_spectr> &value)
{

}

void db_local_state::slot_read_params_spectr()
{
    QVector<params_spectr> result {};

    if(m_dbase.isOpen())
    {
        QStringList str_select_table;

        str_select_table << "SELECT"
                         << list_column()
                         << "FROM"
                         << visual_settings_table;

        QSqlQuery* query = new QSqlQuery(m_dbase);

        if(query->prepare(str_select_table.join(" ")))
        {
            if (query->exec())
            {
                QSqlRecord sql_rec(query->record());

                while(query->next())
                {
                    params_spectr data;

                    data.set_descr(query->value(sql_rec.indexOf("id_params")).toString());
                    data.set_frequency_min(query->value(sql_rec.indexOf("min_freq")).toUInt());
                    data.set_frequency_max(query->value(sql_rec.indexOf("max_freq")).toUInt());
                    data.set_lna_gain(query->value(sql_rec.indexOf("lna_gain")).toUInt());
                    data.set_vga_gain(query->value(sql_rec.indexOf("vga_gain")).toUInt());
                    data.set_fft_bin_width(query->value(sql_rec.indexOf("fft_bin_width")).toUInt());
                    data.set_descr(query->value(sql_rec.indexOf("descr")).toString());

                    result.append(data);
                }
            }
        }else{
            m_str_error_dbase = query->lastError().text();
#ifdef QT_DEBUG
            qDebug() << Q_FUNC_INFO << tr("error:") << query->lastError().text();
            qDebug() << Q_FUNC_INFO << tr("last query:") << query->lastQuery();
#endif
        }
    }
    emit signal_read_params_spectr(result);
}

void db_local_state::slot_close_db()
{
    if(m_dbase.isOpen())
        m_dbase.close();
}

bool db_local_state::create_table(const QString &table_name) const
{
    if(m_dbase.isOpen()&&(!table_name.isEmpty()))
    {
        QStringList str_create_table;

        str_create_table << "CREATE TABLE"
                         << table_name
                         << "(" << list_column_and_type(table_name) << ");";

        QSqlQuery query(m_dbase);
        QString sql(str_create_table.join(" "));

        if(query.exec(sql)){
#ifdef QT_DEBUG
            qDebug() << Q_FUNC_INFO << "create table:" << table_name;
#endif
            return true;
        } else {
#ifdef QT_DEBUG
            qDebug() << Q_FUNC_INFO << "can't create table:" << table_name;
            qDebug() << Q_FUNC_INFO << tr("error:") << query.lastError().text();
            qDebug() << Q_FUNC_INFO << tr("last query:") << query.lastQuery();
#endif
            return false;
        }
    }

    return false;
}

bool db_local_state::is_table_name_resolve(const QString &table_name) const
{
    if(m_dbase.isOpen()&&(!table_name.isEmpty()))
    {
        QSqlQuery query(m_dbase);
        QString sqlQuery = QString("SELECT name FROM sqlite_master WHERE type =:table AND name = '%1' ").arg(table_name);
        query.prepare(sqlQuery);
        query.bindValue(":table", "table");
        query.exec();
        int field_no = query.record().indexOf("name");

        while (query.next()) {
            QString _name = query.value(field_no).toString();

            // если таблица существует
            if(_name.contains(table_name, Qt::CaseInsensitive))
                return true;
        }
        return false;
    }
    return false;
}

QString db_local_state::list_column_and_type(const QString &table_name) const
{
    QStringList list;
    QMap<QString, QString> tmp_column = m_table.value(table_name);

    for(int i=0; i<tmp_column.keys().size(); i++)
    {
        QString tmp;
        tmp.append(tmp_column.keys().at(i));
        tmp.append(" ");
        tmp.append(tmp_column.value(tmp_column.keys().at(i)));
        list.append(tmp);
    }

    return list.join(",");
}

QString db_local_state::list_column(const QString &prefix)
{
    QStringList list;

    for(int i=0; i<m_column_visual_settings.keys().size(); ++i)
    {
        QString tmp;

        if(!prefix.isEmpty())
            tmp.append(prefix);

        tmp.append(m_column_visual_settings.keys().at(i));

        list.append(tmp);
    }

    return list.join(",");
}

void db_local_state::set_pragma(const QString &param, const QString &value)
{
    QString sql_query = QString("PRAGMA %1 = %2").arg(param).arg(value);

    if (m_dbase.isOpen())
    {
        QSqlQuery query(m_dbase);
        query.prepare(sql_query);
        if(!query.exec())
        {
            m_str_error_dbase = query.lastError().text();
#ifdef QT_DEBUG
            qDebug() << Q_FUNC_INFO << tr("error:") << m_str_error_dbase;
            qDebug() << Q_FUNC_INFO << tr("last query:") << query.lastQuery();
#endif
        }
    }
}

bool db_local_state::start_transaction()
{
    bool on(false);
    if(m_dbase.isOpen())
        on = m_dbase.transaction();
    return on;
}

bool db_local_state::commit_transaction()
{
    bool on(false);
    if(m_dbase.isOpen())
        on = m_dbase.commit();
    return on;
}
