#ifndef SWEEPWORKER_H
#define SWEEPWORKER_H

#include <QObject>

class SweepWorker : public QObject
{
    Q_OBJECT
public:
    explicit SweepWorker(QObject *parent = nullptr);

signals:

public slots:
};

#endif // SWEEPWORKER_H