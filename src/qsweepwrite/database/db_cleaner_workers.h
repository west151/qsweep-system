#ifndef DB_CLEANER_WORKERS_H
#define DB_CLEANER_WORKERS_H

#include "db_custom_workers.h"

class db_cleaner_workers : public db_custom_workers
{
    Q_OBJECT
public:
    explicit db_cleaner_workers(QObject *parent = nullptr);

public slots:
    void slot_clean_db(const QString &);

};

#endif // DB_CLEANER_WORKERS_H
