#ifndef QSWEEPMESSAGELOG_H
#define QSWEEPMESSAGELOG_H

#include <QtCore/qshareddata.h>
#include <QtCore/qmetatype.h>

#include "qsweepprotocol_global.h"

class QSweepMessageLogData;

class QSWEEPPROTOCOLSHARED_EXPORT QSweepMessageLog
{
public:
    QSweepMessageLog();
    QSweepMessageLog(const QSweepMessageLog &);
    QSweepMessageLog(const QByteArray &json, const bool binary = false);
    QSweepMessageLog &operator=(const QSweepMessageLog &);
    ~QSweepMessageLog();

    bool isValid() const;

    QString textMessage()const;
    void setTextMessage(const QString &);

    QDateTime dateTime()const;

    QByteArray exportToJson(const bool binary = false) const;

private:
    QSharedDataPointer<QSweepMessageLogData> data;
};

Q_DECLARE_METATYPE(QSweepMessageLog)

#endif // QSWEEPMESSAGELOG_H
