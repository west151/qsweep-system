#ifndef QSWEEPREQUEST_H
#define QSWEEPREQUEST_H

#include <QObject>
#include <QDateTime>

#include "qsweepprotocol_global.h"

enum class TypeRequest: qint32 {
    UNKNOWN = 0,
    INFO,
    SWEEP_SPECTR
};

class QSWEEPPROTOCOLSHARED_EXPORT QSweepRequest : public QObject
{
    Q_OBJECT
public:
    explicit QSweepRequest(QObject *parent = nullptr);
    QSweepRequest(const QByteArray &json, bool binary = false);

    bool isValid()const;

    QString idRequest()const;

    void setTypeRequest(const TypeRequest &);
    TypeRequest typeRequest()const;

    void setDateTimeRequest(const QDateTime &value = QDateTime::currentDateTimeUtc());
    QDateTime dateTimeRequest()const;

    QByteArray exportToJson() const;
    QByteArray exportToJsonBinary() const;

private:
    bool m_valid;
    QString m_id;
    TypeRequest m_typeRequest;
    QDateTime m_dateTimeRequest;
};

#endif // QSWEEPREQUEST_H
