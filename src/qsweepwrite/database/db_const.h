#ifndef DB_CONST_H
#define DB_CONST_H

#include <QString>
#include <QMap>
#include <QDir>
#include <QDataStream>

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
    {"data_params", sqlite_type_blob}
};

static const QMap<QString, QString> column_spectr_data
{
    {"id_pk", sqlite_type_integer_pk},
    {"params_id", sqlite_type_char.arg(8)},
    {"data_spectr", sqlite_type_blob}
};

static const QMap<QString, QMap<QString, QString> > table
{
    {spectr_params_table, column_spectr_params},
    {spectr_data_table, column_spectr_data}
};

QStringList list_of_all_chunk_files(const QString &db_path, const qint32 &db_file_count);
QString pragma_sql(const QString &param, const QString &value);
QString list_column_and_type(const QString &table_name);
QString list_column_prefix(const QString &table_name, const QString &prefix);
QString create_table_sql(const QString &table_name);
QString insert_table_sql(const QString &table_name);
QString format_size(const qint64 &size);

template<typename data_type>
QByteArray toByteArray(data_type data) {
    QByteArray bytes;
    QDataStream stream(&bytes, QIODevice::WriteOnly);
    stream << data;
    return bytes;
}

#endif // DB_CONST_H
