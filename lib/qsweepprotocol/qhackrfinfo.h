#ifndef QHACKRFINFO_H
#define QHACKRFINFO_H

#include <QObject>

#include "qsweepprotocol_global.h"

class QSWEEPPROTOCOLSHARED_EXPORT QHackrfInfo : public QObject
{
    Q_OBJECT
public:
    explicit QHackrfInfo(QObject *parent = nullptr);
    QHackrfInfo(const QByteArray &json, bool binary = false);

    bool isValid()const;

    void setIndexBoard(const qint32 &);
    qint32 indexBoard()const;

    void setSerialNumbers(const QString &);
    QString serialNumbers()const;

    QByteArray exportToJson() const;
    QByteArray exportToJsonBinary() const;

private:
    bool m_valid;
    qint32 m_index;
    QString m_serialNumbers;
};

#endif // QHACKRFINFO_H
