#ifndef QSWEEPMESSAGELOG_H
#define QSWEEPMESSAGELOG_H

#include <QObject>

class QSweepMessageLog : public QObject
{
    Q_OBJECT
public:
    explicit QSweepMessageLog(QObject *parent = nullptr);

signals:

public slots:
};

#endif // QSWEEPMESSAGELOG_H