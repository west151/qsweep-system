#ifndef DB_WRITER_H
#define DB_WRITER_H

#include <QObject>

class db_writer : public QObject
{
    Q_OBJECT
public:
    explicit db_writer(QObject *parent = nullptr);

signals:

public slots:
};

#endif // DB_WRITER_H