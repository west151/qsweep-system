#ifndef QSWEEPREQUEST_H
#define QSWEEPREQUEST_H

#include <QObject>

#include "qsweepprotocol_global.h"

class QSWEEPPROTOCOLSHARED_EXPORT QSweepRequest : public QObject
{
    Q_OBJECT
public:
    explicit QSweepRequest(QObject *parent = nullptr);
    QSweepRequest(const QByteArray &json, bool binary);

    QString idRequest()const;

    QByteArray exportToJson() const;
    QByteArray exportToJsonBinary() const;

private:
    QString m_id;
};

#endif // QSWEEPREQUEST_H
