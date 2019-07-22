#include "ranges_template.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QPair>
#include <QList>

static const QString RANGE_NAME_KEY = QStringLiteral("name");
static const QString RANGE_DESCR_KEY = QStringLiteral("descr");
static const QString FREQ_RANGE_KEY = QStringLiteral("freq_ranges");
static const QString BEG_FREQ_KEY = QStringLiteral("beg_freq");
static const QString END_FREQ_KEY = QStringLiteral("end_freq");

class ranges_template_data : public QSharedData {
public:
    ranges_template_data(): QSharedData()
    {
        valid = false;
        name.clear();
        descr.clear();
        freq_ranges.clear();
    }
    ranges_template_data(const ranges_template_data &other) : QSharedData(other)
    {
        valid = other.valid;
        name = other.name;
        descr = other.descr;
        freq_ranges = other.freq_ranges;
    }

    ~ranges_template_data() {}

    bool valid;
    QString name;
    QString descr;
    QList<QPair<quint64, quint64> > freq_ranges;
};

ranges_template::ranges_template() : data(new ranges_template_data)
{
}

ranges_template::ranges_template(const ranges_template &rhs) : data(rhs.data)
{
}

ranges_template::ranges_template(const QByteArray &json, const bool binary) : data(new ranges_template_data)
{
    QJsonDocument doc;
    if (binary)
        doc = QJsonDocument::fromBinaryData(json, QJsonDocument::BypassValidation);
    else
        doc = QJsonDocument::fromJson(json);

    const QJsonObject json_object(doc.object());

    data->name = json_object.value(RANGE_NAME_KEY).toString();
    data->descr = json_object.value(RANGE_DESCR_KEY).toString();

    for(const QJsonValue value: json_object.value(FREQ_RANGE_KEY).toArray())
    {
        QPair<quint64, quint64> freq_range;
        const QJsonObject range_json(value.toObject());

        freq_range.first = range_json.value(BEG_FREQ_KEY).toString().toULongLong();
        freq_range.second = range_json.value(END_FREQ_KEY).toString().toULongLong();

        data->freq_ranges.append(freq_range);
    }

    if(!doc.isEmpty())
        data->valid = true;
    else
        data->valid = false;
}

ranges_template &ranges_template::operator=(const ranges_template &rhs)
{
    if (this != &rhs) {
        data.operator=(rhs.data);
    }
    return *this;
}

ranges_template::~ranges_template()
{
}

bool ranges_template::is_valid() const
{
    return data->valid;
}

void ranges_template::set_name_ranges(const QString &value)
{
    data->name = value;
}

QString ranges_template::name_ranges() const
{
    return data->name;
}

void ranges_template::set_descr_ranges(const QString &value)
{
    data->descr = value;
}

QString ranges_template::descr_ranges() const
{
    return data->descr;
}

void ranges_template::set_freq_ranges(const QList<QPair<quint64, quint64> > &value)
{
    data->freq_ranges = value;
}

const QList<QPair<quint64, quint64> > ranges_template::freq_ranges() const
{
    return data->freq_ranges;
}
