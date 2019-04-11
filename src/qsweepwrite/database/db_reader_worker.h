#ifndef DB_READER_WORKER_H
#define DB_READER_WORKER_H

#include <QObject>

class db_reader_worker : public QObject
{
    Q_OBJECT
public:
    explicit db_reader_worker(QObject *parent = nullptr);

signals:

public slots:
};

#endif // DB_READER_WORKER_H
