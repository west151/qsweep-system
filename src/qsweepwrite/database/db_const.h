#ifndef DB_CONST_H
#define DB_CONST_H

#include <QString>
#include <QMap>
#include <QDir>

static const QString database_driver = "QSQLITE";

static const QString connection_write = "data_write";
static const QString connection_read = "data_read";
static const QString connection_system = "ctrl_system";

// spectr result table name
static const QString spectr_data_table = "spectr_data_tbl";
// spectr params table name
static const QString spectr_params_table = "spectr_params_tbl";

// result database (data) (template)
static const QString result_database_name = "db_chunk_%1.sqlite";

// SQLITE data type
static const QString sqlite_type_numeric(QString("NUMERIC"));
static const QString sqlite_type_integer(QString("INTEGER"));
static const QString sqlite_type_integer_pk(QString("INTEGER PRIMARY KEY"));
static const QString sqlite_type_char(QString("CHAR(%1)"));
static const QString sqlite_type_varchar(QString("VARCHAR(%1)"));
static const QString sqlite_type_floatnone(QString("FLOATNONE"));
static const QString sqlite_type_datetime(QString("DATETIME"));
static const QString sqlite_type_double(QString("DOUBLE PRECISION"));
static const QString sqlite_type_blob(QString("BLOB"));

static const QMap<QString, QString> column_spectr_params
{
    {"id_pk", sqlite_type_integer_pk},
    {"params_id", sqlite_type_char.arg(8)},
    {"min_freq", sqlite_type_double},
    {"max_freq", sqlite_type_double},
    {"params_dt", sqlite_type_datetime},
    {"lna_gain", sqlite_type_integer},
    {"vga_gain", sqlite_type_integer},
    {"fft_bin_width", sqlite_type_double}
};

static const QMap<QString, QString> column_spectr_data
{
    {"id_pk", sqlite_type_integer_pk},
    {"params_id", sqlite_type_char.arg(8)},
    {"dt", sqlite_type_datetime},
    {"power_spectr", sqlite_type_blob}
};

static const QMap<QString, QMap<QString, QString> > table
{
    {spectr_params_table, column_spectr_params},
    {spectr_data_table, column_spectr_data}
};

QStringList list_of_all_chunk_files(const QString &db_path, const qint32 &db_chunk)
{
    QStringList list_chunk;

    if((!db_path.isEmpty())&&(db_chunk>0)){
        for(int i=0; i<db_chunk; ++i ){
            QString file = db_path + QDir::separator() + result_database_name.arg(i+1);
            list_chunk.append(file);
        }
    }

    if((db_path.isEmpty())&&(db_chunk>0)){
        for(int i=0; i<db_chunk; ++i ){
            QString file = result_database_name.arg(i+1);
            list_chunk.append(file);
        }
    }

    return list_chunk;
}

QString pragma_sql(const QString &param, const QString &value)
{
    QString pragma_string;

    if((!param.isEmpty())&&(!value.isEmpty()))
        pragma_string = QString("PRAGMA %1 = %2").arg(param).arg(value);

    return pragma_string;
}

QString list_column_and_type(const QString &table_name)
{
    QStringList list;
    QMap<QString, QString> tmp_column = table.value(table_name);

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

QString list_column_prefix(const QString &table_name, const QString &prefix)
{
    QStringList list;
    QMap<QString, QString> tmp_column = table.value(table_name);

    for(int i=0; i<tmp_column.keys().size(); ++i)
    {
        QString tmp;

        if(!prefix.isEmpty())
            tmp.append(prefix);

        tmp.append(tmp_column.keys().at(i));

        list.append(tmp);
    }

    return list.join(",");
}

QString create_table_sql(const QString &table_name)
{
    QString sql;

    if((!table_name.isEmpty()&&(!table.value(table_name).isEmpty())))
    {
        QStringList str_create_table;

        str_create_table << "CREATE TABLE"
                         << table_name
                         << "(" << list_column_and_type(table_name) << ");";

        sql.append(str_create_table.join(" "));
    }

    return sql;
}

QString insert_table_sql(const QString &table_name)
{
    QString sql;

    return sql;
}

//void db_local_state::slot_write_params_spectr(const QVector<params_spectr> &value)
//{
//    if((value.size()>0)&&(m_dbase.isOpen()))
//    {
//        QStringList str_insert_table;

//        str_insert_table << "INSERT INTO"
//                         << visual_settings_table
//                         << "(" << list_column() << ")"
//                         << "VALUES"
//                         << "(" << list_column(":") << ");";

//        QSqlQuery* query = new QSqlQuery(m_dbase);

//        QString str_insert_sql(str_insert_table.join(" "));
//        query->prepare(str_insert_sql);

//        start_transaction();

//        for(int i=0; i<value.size(); ++i)
//        {
//            query->bindValue(":id_params", value.at(i).id_params_spectr());
//            query->bindValue(":min_freq", value.at(i).frequency_min());
//            query->bindValue(":max_freq", value.at(i).frequency_max());
//            query->bindValue(":lna_gain", value.at(i).lna_gain());
//            query->bindValue(":vga_gain", value.at(i).vga_gain());
//            query->bindValue(":fft_bin_width", value.at(i).fft_bin_width());
//            query->bindValue(":descr", value.at(i).descr());
//        }

//        bool on = query->exec();

//        if(!on)
//            update_last_error(query);

//        if(!commit_transaction())
//            m_dbase.rollback();
//    }
//}

//void db_local_state::slot_read_params_spectr()
//{
//    QVector<params_spectr> result {};

//    if(m_dbase.isOpen())
//    {
//        QStringList str_select_table;

//        str_select_table << "SELECT"
//                         << list_column()
//                         << "FROM"
//                         << visual_settings_table;

//        QSqlQuery* query = new QSqlQuery(m_dbase);

//        if(query->prepare(str_select_table.join(" ")))
//        {
//            if (query->exec())
//            {
//                QSqlRecord sql_rec(query->record());

//                while(query->next())
//                {
//                    params_spectr data;

//                    data.set_id_params_spectr(query->value(sql_rec.indexOf("id_params")).toString());
//                    data.set_frequency_min(query->value(sql_rec.indexOf("min_freq")).toUInt());
//                    data.set_frequency_max(query->value(sql_rec.indexOf("max_freq")).toUInt());
//                    data.set_lna_gain(query->value(sql_rec.indexOf("lna_gain")).toUInt());
//                    data.set_vga_gain(query->value(sql_rec.indexOf("vga_gain")).toUInt());
//                    data.set_fft_bin_width(query->value(sql_rec.indexOf("fft_bin_width")).toUInt());
//                    data.set_descr(query->value(sql_rec.indexOf("descr")).toString());

//                    result.append(data);
//                }
//            }
//        }else
//            update_last_error(query);
//    }
//    emit signal_read_params_spectr(result);
//}

//void db_local_state::slot_remove_params_spectr(const QString &id_params)
//{
//    if((!id_params.isEmpty())&&(m_dbase.isOpen()))
//    {
//        QStringList str_delete_table;

//        str_delete_table << "DELETE FROM"
//                         << visual_settings_table
//                         << "WHERE"
//                         << "id_params"
//                         << "="
//                         << ":id_params";

//        QSqlQuery* query = new QSqlQuery(m_dbase);

//        QString str_insert_sql(str_delete_table.join(" "));
//        query->prepare(str_insert_sql);
//        query->bindValue(":id_params", id_params);

//        if(start_transaction())
//        {
//            if(!query->exec())
//                update_last_error(query);

//            if(!commit_transaction())
//            {
//                m_dbase.rollback();
//                update_last_error(query);
//            }
//        }
//    }
//}


#endif // DB_CONST_H
