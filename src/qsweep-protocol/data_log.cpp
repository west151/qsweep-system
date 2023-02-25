#include "data_log.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>

#include "constkeys.h"

class data_log_data : public QSharedData {
public:
    data_log_data(): QSharedData()
    {
        m_valid = false;
        m_text_message.clear();
        m_date_time = QDateTime::currentDateTimeUtc();
    }
    data_log_data(const data_log_data &other) : QSharedData(other)
    {
        m_valid = other.m_valid;
        m_text_message = other.m_text_message;
        m_date_time = other.m_date_time;
    }

    ~data_log_data() {}

    bool m_valid;
    QDateTime m_date_time;
    QString m_text_message;
};

data_log::data_log() : data(new data_log_data)
{
}

data_log::data_log(const data_log &rhs) : data(rhs.data)
{
}

data_log::data_log(const QByteArray &json) : data(new data_log_data)
{
    QJsonDocument doc;

    doc = QJsonDocument::fromJson(json);

    const QJsonObject jsonObject(doc.object());
    data->m_text_message = jsonObject.value(TEXT_MESSAGE_KEY).toString();
    auto dt = QDateTime::fromString(jsonObject.value(DT_KEY).toString(), DT_FORMAT);
    dt.setTimeSpec(Qt::UTC);
    data->m_date_time = dt;

    if(!doc.isEmpty())
        data->m_valid = true;
    else
        data->m_valid = false;
}

data_log &data_log::operator=(const data_log &rhs)
{
    if (this != &rhs) {
        data.operator=(rhs.data);
    }
    return *this;
}

data_log::~data_log()
{
}

bool data_log::is_valid() const
{
    return data->m_valid;
}

QString data_log::text_message() const
{
    return data->m_text_message;
}

void data_log::set_text_message(const QString &value)
{
    data->m_text_message = value;
}

QDateTime data_log::date_time() const
{
    return data->m_date_time;
}

QByteArray data_log::to_json() const
{
    QJsonObject jsonObject;
    jsonObject.insert(TEXT_MESSAGE_KEY, data->m_text_message);
    jsonObject.insert(DT_KEY, data->m_date_time.toUTC().toString(DT_FORMAT));

    const QJsonDocument doc(jsonObject);

    return doc.toJson(QJsonDocument::Compact);
}
