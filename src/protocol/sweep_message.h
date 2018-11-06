#ifndef SWEEP_MESSAGE_H
#define SWEEP_MESSAGE_H

#include <QSharedData>
#include <QMetaType>

enum class type_message: qint32 {
    UNKNOWN = 0,
    CTRL_INFO,
    DATA_INFO,
    CTRL_SPECTR,
    DATA_SPECTR,
    DATA_MESSAGE_LOG,
    DATA_SYSTEM_MONITOR
};

class sweep_message_data;

class sweep_message
{
public:
    sweep_message();
    sweep_message(const sweep_message &);
    sweep_message(const QByteArray &json, const bool binary = false);
    sweep_message &operator=(const sweep_message &);
    ~sweep_message();

    bool is_valid() const;

    QString id_message()const;

    void set_type(const type_message &);
    type_message type()const;

    void set_data_message(const QByteArray &);
    QByteArray data_message()const;

    QByteArray export_json(const bool binary = false) const;

private:
    QSharedDataPointer<sweep_message_data> data;
};

Q_DECLARE_METATYPE(sweep_message)

#endif // SWEEP_MESSAGE_H
