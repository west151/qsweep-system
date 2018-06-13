#ifndef QSWEEPANSWER_H
#define QSWEEPANSWER_H

#include <QObject>
#include <QDateTime>

#include "qsweepprotocol_global.h"

enum class TypeAnswer: qint32 {
    UNKNOWN = 0,
    INFO
};

class QSWEEPPROTOCOLSHARED_EXPORT QSweepAnswer : public QObject
{
    Q_OBJECT
public:
    explicit QSweepAnswer(QObject *parent = nullptr);
    QSweepAnswer(const QByteArray &json, bool binary = false);

    bool isValid()const;
    QString idRequest()const;

    void setTypeAnswer(const TypeAnswer &);
    TypeAnswer typeAnswer()const;

    void setDateTimeAnswer(const QDateTime &value = QDateTime::currentDateTimeUtc());
    QDateTime dateTimeAnswer()const;

    QByteArray exportToJson() const;
    QByteArray exportToJsonBinary() const;

private:
    bool m_valid;
    QString m_id;
    TypeAnswer m_typeAnswer;
    QDateTime m_dateTimeAnswer;
};

#endif // QSWEEPANSWER_H
