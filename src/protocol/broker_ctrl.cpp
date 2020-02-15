#include "broker_ctrl.h"

#include <QJsonDocument>
#include <QJsonObject>

#include "constkeys.h"

class broker_ctrl_data : public QSharedData {
public:
    broker_ctrl_data(): QSharedData()
    {
        m_valid = false;
        m_broker_ctrl_type = broker_ctrl_type::unknown;
        m_topic_list.clear();
    }
    broker_ctrl_data(const broker_ctrl_data &other) : QSharedData(other)
    {
        m_valid = other.m_valid;
        m_broker_ctrl_type = other.m_broker_ctrl_type;
        m_topic_list = other.m_topic_list;
    }

    ~broker_ctrl_data() {}

    bool m_valid;
    QStringList m_topic_list;
    broker_ctrl_type m_broker_ctrl_type;
};

broker_ctrl::broker_ctrl() : data(new broker_ctrl_data)
{
}

broker_ctrl::broker_ctrl(const broker_ctrl &rhs) : data(rhs.data)
{
}

broker_ctrl::broker_ctrl(const QByteArray &json) : data(new broker_ctrl_data)
{
    QJsonDocument doc;

    doc = QJsonDocument::fromJson(json);

    const QJsonObject json_object(doc.object());

    data->m_broker_ctrl_type = static_cast<broker_ctrl_type>(json_object.value(BROKER_CTRL_TYPE_KEY).toInt(0));
    const QString topic_list = json_object.value(TOPIC_LIST_KEY).toString();
    data->m_topic_list = topic_list.split(";", QString::SkipEmptyParts);

    if(!doc.isEmpty())
        data->m_valid = true;
    else
        data->m_valid = false;
}

broker_ctrl &broker_ctrl::operator=(const broker_ctrl &rhs)
{
    if (this != &rhs) {
        data.operator=(rhs.data);
    }
    return *this;
}

broker_ctrl::~broker_ctrl()
{
}

bool broker_ctrl::is_valid() const
{
    return data->m_valid;
}

void broker_ctrl::set_ctrl_type(const broker_ctrl_type &value)
{
    data->m_broker_ctrl_type = value;
}

broker_ctrl_type broker_ctrl::ctrl_type() const
{
    return data->m_broker_ctrl_type;
}

void broker_ctrl::set_topic_list(const QStringList &value)
{
    data->m_topic_list = value;
}

QStringList broker_ctrl::topic_list() const
{
    return data->m_topic_list;
}

QByteArray broker_ctrl::to_json() const
{
    QJsonObject json_object;

    json_object.insert(BROKER_CTRL_TYPE_KEY, static_cast<qint32>(data->m_broker_ctrl_type));
    json_object.insert(TOPIC_LIST_KEY, data->m_topic_list.join(";"));

    const QJsonDocument doc(json_object);

    return doc.toJson(QJsonDocument::Compact);
}
