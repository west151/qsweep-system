#ifndef QSWEEPPARAMS_H
#define QSWEEPPARAMS_H

#include <QObject>

#include "qsweepprotocol_global.h"

class QSWEEPPROTOCOLSHARED_EXPORT QSweepParams : public QObject
{
    Q_OBJECT
public:
    explicit QSweepParams(QObject *parent = nullptr);

signals:

public slots:
};

#endif // QSWEEPPARAMS_H
