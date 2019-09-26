#ifndef TOP_MODEL_H
#define TOP_MODEL_H

#include <QObject>
#include <QDateTime>

class QTimer;

class top_model : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString system_time READ system_time NOTIFY signal_time_changed)

public:
    explicit top_model(QObject *parent = nullptr);

    void start();

    QString system_time()const;

public slots:
    void slot_system_time_update();

signals:
    void signal_time_changed();

private:
    QDateTime m_system_time;
    QTimer *m_timer;
};

#endif // TOP_MODEL_H
