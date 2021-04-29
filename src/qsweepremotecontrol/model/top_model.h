#ifndef TOP_MODEL_H
#define TOP_MODEL_H

#include <QObject>
#include <QDateTime>

class QTimer;

class top_model : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString system_time READ system_time NOTIFY signal_time_changed)
    Q_PROPERTY(bool is_connect_brocker READ is_connect_brocker NOTIFY signal_connect_brocker_changed)

public:
    explicit top_model(QObject *parent = nullptr);

    void start();
    QString system_time()const;

    bool is_connect_brocker();

public slots:
    void slot_set_connect_brocker(bool on);

signals:
    void signal_time_changed();
    void signal_connect_brocker_changed();

private:
    bool m_connect_brocker;
    QDateTime m_system_time;
    QTimer *m_timer;

private slots:
    void slot_system_time_update();
};

#endif // TOP_MODEL_H
