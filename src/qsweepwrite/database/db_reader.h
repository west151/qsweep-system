#ifndef DB_READER_H
#define DB_READER_H

#include <QObject>

class db_reader : public QObject
{
    Q_OBJECT
public:
    explicit db_reader(QObject *parent = nullptr);

signals:

public slots:
};

#endif // DB_READER_H