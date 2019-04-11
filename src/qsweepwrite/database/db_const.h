#ifndef DB_CONST_H
#define DB_CONST_H

#include <QString>

static const QString database_driver = "QSQLITE";

static const QString connection_write = "data_write";
static const QString connection_read = "data_read";
static const QString connection_system = "ctrl_system";

// system database
static const QString system_database_name = "db_system.sqlite";
// system table name
static const QString spectr_result_table = "spectr_result_tbl";

// result database (data) (template)
static const QString result_database_name = "db_chunk_%1.sqlite";
// result table name
static const QString spectr_data_table = "spectr_data_tbl";

// SQLITE data type
static const QString sqlite_type_numeric(QString("NUMERIC"));
static const QString sqlite_type_integer(QString("INTEGER"));
static const QString sqlite_type_integer_pk(QString("NTEGER PRIMARY KEY"));
static const QString sqlite_type_char(QString("CHAR(%1)"));
static const QString sqlite_type_varchar(QString("VARCHAR(%1)"));
static const QString sqlite_type_floatnone(QString("FLOATNONE"));
static const QString sqlite_type_datetime(QString("DATETIME"));
static const QString sqlite_type_double(QString("DOUBLE PRECISION"));
static const QString sqlite_type_blob(QString("BLOB"));

#endif // DB_CONST_H
