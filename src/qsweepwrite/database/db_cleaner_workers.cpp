#include "db_cleaner_workers.h"

db_cleaner_workers::db_cleaner_workers(QObject *parent) : db_custom_workers(parent)
{
    setObjectName(this->metaObject()->className());
}