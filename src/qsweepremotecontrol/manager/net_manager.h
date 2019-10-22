#ifndef NET_MANAGER_H
#define NET_MANAGER_H

#include <QObject>

class net_manager : public QObject
{
    Q_OBJECT
public:
    explicit net_manager(QObject *parent = nullptr);

signals:

public slots:
};

#endif // NET_MANAGER_H
