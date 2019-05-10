#include "db_const.h"

QStringList list_of_all_chunk_files(const QString &db_path, const qint32 &db_file_count)
{
    QStringList list_chunk;

    if((!db_path.isEmpty())&&(db_file_count>0)){
        for(int i=0; i<db_file_count; ++i ){
            QString file = db_path + QDir::separator() + result_database_name.arg(i+1);
            list_chunk.append(file);
        }
    }

    if((db_path.isEmpty())&&(db_file_count>0)){
        for(int i=0; i<db_file_count; ++i ){
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

    QStringList str_insert_table;

    str_insert_table << "INSERT INTO"
                     << table_name
                     << "(" << list_column_prefix(table_name, "") << ")"
                     << "VALUES"
                     << "(" << list_column_prefix(table_name, ":") << ");";

    sql.append(str_insert_table.join(" "));

    return sql;
}

QString format_size(const qint64 &size)
{
    QStringList units = {"Bytes", "KB", "MB", "GB", "TB", "PB"};
    int i;
    double output_size = size;
    for(i=0; i<units.size()-1; i++) {
        if(output_size<1024) break;
        output_size= output_size/1024;
    }
    return QString("%0 %1").arg(output_size, 0, 'f', 2).arg(units[i]);
}
