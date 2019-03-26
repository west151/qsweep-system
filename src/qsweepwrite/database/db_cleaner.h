#ifndef DB_CLEANER_H
#define DB_CLEANER_H

#include <QObject>

class db_cleaner : public QObject
{
    Q_OBJECT
public:
    explicit db_cleaner(QObject *parent = nullptr);

signals:

public slots:
};

#endif // DB_CLEANER_H