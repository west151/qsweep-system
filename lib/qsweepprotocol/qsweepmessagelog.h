#ifndef QSWEEPMESSAGELOG_H
#define QSWEEPMESSAGELOG_H

#include <QObject>
#include <QtCore/qdatetime.h>

#include "qsweepprotocol_global.h"

class QSWEEPPROTOCOLSHARED_EXPORT QSweepMessageLog : public QObject
{
    Q_OBJECT
public:
    explicit QSweepMessageLog(QObject *parent = nullptr);
    QSweepMessageLog(const QByteArray &json, bool binary = false);

    bool isValid()const;
    QDateTime dateTime()const;

    QString textMessage()const;
    void setTextMessage(const QString &);

    QByteArray exportToJson(bool binary = false) const;

private:
    bool m_valid;
    QString m_textMessage;
    QDateTime m_dateTime;
};

#endif // QSWEEPMESSAGELOG_H
