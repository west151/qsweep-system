#ifndef TA_SPECTR_H
#define TA_SPECTR_H

#include <QObject>

class data_spectr;

class ta_spectr : public QObject
{
    Q_OBJECT
public:
    explicit ta_spectr(QObject *parent = nullptr);

signals:
    void signal_spectr_rt(const QDateTime &, const quint64 &, const quint64 &, const QVector<qreal> &);

public slots:
    void slot_data_spectr(const data_spectr &);
};

#endif // TA_SPECTR_H
