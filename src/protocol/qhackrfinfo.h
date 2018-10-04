#ifndef QHACKRFINFO_H
#define QHACKRFINFO_H

#include <QtCore/qshareddata.h>
#include <QtCore/qmetatype.h>

class QHackrfInfoData;

class QHackrfInfo
{
public:
    QHackrfInfo();
    QHackrfInfo(const QHackrfInfo &);
    QHackrfInfo(const QByteArray &json, const bool binary = false);
    QHackrfInfo &operator=(const QHackrfInfo &);
    ~QHackrfInfo();

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

    bool isValid() const;

    QByteArray exportToJson(const bool binary = false) const;

private:
    QSharedDataPointer<QHackrfInfoData> data;
};

Q_DECLARE_METATYPE(QHackrfInfo)

#endif // QHACKRFINFO_H
