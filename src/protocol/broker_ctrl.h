#ifndef BROKER_CTRL_H
#define BROKER_CTRL_H

#include <QSharedData>
#include <QMetaType>
#include <QStringList>

enum broker_ctrl_type: qint32 {
    unknown,
    subscribe,
    unsubscribe
};

class broker_ctrl_data;

class broker_ctrl
{
public:
    broker_ctrl();
    broker_ctrl(const broker_ctrl &);
    broker_ctrl(const QByteArray &json);
    broker_ctrl &operator=(const broker_ctrl &);
    ~broker_ctrl();

    bool is_valid() const;

    void set_ctrl_type(const broker_ctrl_type &);
    broker_ctrl_type ctrl_type()const;

    void set_topic_list(const QStringList &);
    QStringList topic_list()const;

    QByteArray to_json() const;

private:
    QSharedDataPointer<broker_ctrl_data> data;
};

Q_DECLARE_METATYPE(broker_ctrl)

#endif // BROKER_CTRL_H
