#include "sweep_message.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QUuid>

#include "constkeys.h"

class sweep_message_data : public QSharedData {
public:
    sweep_message_data(): QSharedData()
    {
        m_valid = false;
        m_id = QUuid::createUuid().toString().mid(1, 8);
        m_data.clear();
        m_type = type_message::unknown;
    }
    sweep_message_data(const sweep_message_data &other) : QSharedData(other)
    {
        m_valid = other.m_valid;
        m_id = other.m_id;
        m_data = other.m_data;
        m_type = other.m_type;
    }

    ~sweep_message_data() {}

    bool m_valid;
    QString m_id;
    QByteArray m_data;
    type_message m_type;
};

sweep_message::sweep_message() : data(new sweep_message_data)
{
}

sweep_message::sweep_message(const sweep_message &rhs) : data(rhs.data)
{
}

sweep_message &sweep_message::operator=(const sweep_message &rhs)
{
    if (this != &rhs) {
        data.operator=(rhs.data);
    }
    return *this;
}

sweep_message::sweep_message(const QByteArray &json) : data(new sweep_message_data)
{
    QJsonDocument doc;

    doc = QJsonDocument::fromJson(json);

    const QJsonObject json_object(doc.object());
    data->m_id = json_object.value(ID_KEY).toString();
    data->m_type = static_cast<type_message>(json_object.value(TYPE_MESSAGE_KEY).toInt(0));

    QByteArray ba;
    ba.append(json_object.value(DATA_KEY).toString());
    data->m_data = QByteArray::fromBase64(ba);

    if(!doc.isEmpty())
        data->m_valid = true;
    else
        data->m_valid = false;
}

sweep_message::~sweep_message()
{
}

bool sweep_message::is_valid() const
{
    return data->m_valid;
}

QString sweep_message::id_message() const
{
    return data->m_id;
}

void sweep_message::set_type(const type_message &value)
{
    data->m_type = value;
}

type_message sweep_message::type() const
{
    return data->m_type;
}

void sweep_message::set_data_message(const QByteArray &value)
{
    data->m_data = value;
}

QByteArray sweep_message::data_message() const
{
    return  data->m_data;
}

QByteArray sweep_message::to_json() const
{
    QJsonObject json_object;

    json_object.insert(ID_KEY, data->m_id);
    json_object.insert(TYPE_MESSAGE_KEY, static_cast<qint32>(data->m_type));
    json_object.insert(DATA_KEY, QString(data->m_data.toBase64()));

    const QJsonDocument doc(json_object);

    return doc.toJson(QJsonDocument::Compact);
}
