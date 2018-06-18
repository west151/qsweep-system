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

    void setBoardID(const QString &);
    QString boardID()const;

    void setFirmwareVersion(const QString &);
    QString firmwareVersion()const;

    void setPartIDNumber(const QString &);
    QString partIDNumber()const;

    void setLibHackrfVersion(const QString &);
    QString libHackrfVersion() const;

    QByteArray exportToJson(bool binary = false) const;

private:
    bool m_valid;
    qint32 m_index;
    QString m_serialNumbers;
    QString m_boardID;
    QString m_firmwareVersion;
    QString m_partIDNumber;
    QString m_libHackrfVersion;
};

#endif // QHACKRFINFO_H
