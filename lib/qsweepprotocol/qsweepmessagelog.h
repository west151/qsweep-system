#ifndef QSWEEPMESSAGELOG_H
#define QSWEEPMESSAGELOG_H

#include <QObject>

#include "qsweepprotocol_global.h"

class QSWEEPPROTOCOLSHARED_EXPORT QSweepMessageLog : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString textMessage READ textMessage WRITE setTextMessage NOTIFY textMessageChanged)

public:
    explicit QSweepMessageLog(QObject *parent = nullptr);

    QString textMessage()const;

signals:
    void textMessageChanged(const QString &textMessage);

public slots:
    void setTextMessage(const QString &textMessage);

    QByteArray exportToJson() const;
    QByteArray exportToJsonBinary() const;

private:
    QString m_id;
    QString m_textMessage;
};

#endif // QSWEEPMESSAGELOG_H
