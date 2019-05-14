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

qint64 dir_size(const QString &dir_path)
{
    qint64 size = 0;
    QDir dir(dir_path);
    //calculate total size of current directories' files
    QDir::Filters file_filters = QDir::Files|QDir::System|QDir::Hidden;
    for(QString file_path : dir.entryList(file_filters)) {
        QFileInfo fi(dir, file_path);
        size+= fi.size();
    }
    //add size of child directories recursively
    QDir::Filters dir_filters = QDir::Dirs|QDir::NoDotAndDotDot|QDir::System|QDir::Hidden;
    for(QString child_dir_path : dir.entryList(dir_filters))
        size+= dir_size(dir_path + QDir::separator() + child_dir_path);
    return size;
}

bool file_compress(const QString &in_file, const QString &out_file, int compressionLevel)
{
    bool is_compressed(false);

    if((!in_file.isEmpty())&&(!out_file.isEmpty()))
    {
        QFile in_file_read(in_file);
        QFile out_file_write(out_file);

        in_file_read.open(QIODevice::ReadOnly);
        out_file_write.open(QIODevice::WriteOnly);

        if(in_file_read.isOpen()&&out_file_write.isOpen())
        {
            QByteArray uncompressed_data = in_file_read.readAll();
            QByteArray compressed_data = qCompress(uncompressed_data, compressionLevel);

            out_file_write.write(compressed_data);
        }

        if(in_file_read.isOpen())
            in_file_read.close();

        if(out_file_write.isOpen())
            out_file_write.close();
    }

    return is_compressed;
}

bool file_uncompressed(const QString &in_file, const QString &out_file)
{
    bool is_uncompressed(false);

    QFile in_file_read(in_file);
    QFile out_file_write(out_file);

    in_file_read.open(QIODevice::ReadOnly);
    out_file_write.open(QIODevice::WriteOnly);

    if(in_file_read.isOpen()&&out_file_write.isOpen())
    {
        QByteArray compressed_data = in_file_read.readAll();
        QByteArray uncompressed_data = qUncompress(compressed_data);

        out_file_write.write(uncompressed_data);
    }

    if(in_file_read.isOpen())
        in_file_read.close();

    if(out_file_write.isOpen())
        out_file_write.close();

    return is_uncompressed;
}
