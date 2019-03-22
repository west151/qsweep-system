#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include <QObject>

class db_manager : public QObject
{
    Q_OBJECT
public:
    explicit db_manager(QObject *parent = nullptr);

signals:

public slots:
};

#endif // DB_MANAGER_H
