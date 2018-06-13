#ifndef QSWEEPREQUEST_H
#define QSWEEPREQUEST_H

#include <QObject>

#include "qsweepprotocol_global.h"

enum class TypeRequest: qint32 {
    UNKNOWN = 0,
    INFO
};

class QSWEEPPROTOCOLSHARED_EXPORT QSweepRequest : public QObject
{
    Q_OBJECT
public:
    explicit QSweepRequest(QObject *parent = nullptr);
    QSweepRequest(const QByteArray &json, bool binary);

    QString idRequest()const;

    void setTypeRequest(const TypeRequest &);
    TypeRequest typeRequest()const;

    QByteArray exportToJson() const;
    QByteArray exportToJsonBinary() const;

private:
    QString m_id;
    TypeRequest m_typeRequest = TypeRequest::UNKNOWN;
};

#endif // QSWEEPREQUEST_H
