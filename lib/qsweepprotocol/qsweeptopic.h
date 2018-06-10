#ifndef QSWEEPTOPIC_H
#define QSWEEPTOPIC_H

#include <QObject>

#include "qsweepprotocol_global.h"

class QSWEEPPROTOCOLSHARED_EXPORT QSweepTopic : public QObject
{
    Q_OBJECT
public:
    enum Topic : qint32 {
        TOPIC_UNKNOWN,
        TOPIC_MESSAGE_LOG,
        TOPIC_CTRL,
        TOPIC_INFO,
        TOPIC_DATA,
        TOPIC_SPECTR
    };

    explicit QSweepTopic(QObject *parent = nullptr);

    QString sweepTopic(const Topic value = TOPIC_UNKNOWN)const;
};

#endif // QSWEEPTOPIC_H
