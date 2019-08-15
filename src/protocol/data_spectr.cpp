#include "data_spectr.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>

#include "constkeys.h"

class data_spectr_data : public QSharedData {
public:
    data_spectr_data(): QSharedData()
    {
        m_valid = false;
        m_powers.clear();
        m_id_params.clear();
    }
    data_spectr_data(const data_spectr_data &other) : QSharedData(other)
    {
        m_valid = other.m_valid;
        m_id_params = other.m_id_params;
        m_powers = other.m_powers;
    }

    ~data_spectr_data() {}

    bool m_valid;
    QString m_id_params;
    QVector<power_spectr> m_powers;
};

data_spectr::data_spectr() : data(new data_spectr_data)
{
}

data_spectr::data_spectr(const data_spectr &rhs) : data(rhs.data)
{
}

data_spectr::data_spectr(const QByteArray &json, const bool binary) : data(new data_spectr_data)
{
    QJsonDocument doc;
    if (binary)
        doc = QJsonDocument::fromBinaryData(json, QJsonDocument::BypassValidation);
    else
        doc = QJsonDocument::fromJson(json);

    const QJsonObject json_object(doc.object());

    for(const QJsonValue &value: json_object.value(POWERS_KEY).toArray())
    {
        power_spectr powerSpectr;
        const QJsonObject objectPowerSpectr(value.toObject());

        // DateTime
        auto dt = QDateTime::fromString(objectPowerSpectr.value(DT_KEY).toString(), DT_FORMAT);
        dt.setTimeSpec(Qt::UTC);
        powerSpectr.m_date_time = dt;
        powerSpectr.m_fft_bin_width = objectPowerSpectr.value(FFT_BIN_WIDTH_KEY).toString().toDouble();
        powerSpectr.num_samples = objectPowerSpectr.value(NUM_SAMPLES_KEY).toString().toUInt();
        powerSpectr.hz_low = objectPowerSpectr.value(FREQUENCY_MIN_KEY).toString().toULongLong();
        powerSpectr.hz_high = objectPowerSpectr.value(FREQUENCY_MAX_KEY).toString().toULongLong();

        const QString valuePower = objectPowerSpectr.value(DATA_KEY).toString();
        QStringList listValue(valuePower.split(";", QString::SkipEmptyParts));

        for(const auto &strItem : listValue){
            const auto value = static_cast<qreal>(strItem.trimmed().toFloat());
            powerSpectr.m_power.append(value);
        }        
        data->m_powers.append(powerSpectr);       
    }

    data->m_id_params = json_object.value(ID_PARAMS_KEY).toString();

    if(!doc.isEmpty())
        data->m_valid = true;
    else
        data->m_valid = false;
}

data_spectr &data_spectr::operator=(const data_spectr &rhs)
{
    if (this != &rhs) {
        data.operator=(rhs.data);
    }
    return *this;
}

data_spectr::~data_spectr()
{
}

bool data_spectr::is_valid() const
{
    return data->m_valid;
}

void data_spectr::set_id_params(const QString &value)
{
    data->m_id_params = value;
}

QString data_spectr::id_params() const
{
    return data->m_id_params;
}

void data_spectr::set_spectr(const QVector<power_spectr> &value)
{
    data->m_powers = value;
}

QVector<power_spectr> data_spectr::spectr() const
{
    return data->m_powers;
}

QByteArray data_spectr::to_json(const bool binary) const
{
    QJsonObject json_object;

    if(data->m_powers.count())
    {
        QJsonArray array;

        for(int i=0; i < data->m_powers.count(); ++i){
            QJsonObject objectPowerSpectr;
            const power_spectr powerSpectr = data->m_powers.at(i);

            objectPowerSpectr.insert(DT_KEY, powerSpectr.m_date_time.toUTC().toString(DT_FORMAT));
            objectPowerSpectr.insert(FREQUENCY_MIN_KEY, QString::number(powerSpectr.hz_low));
            objectPowerSpectr.insert(FREQUENCY_MAX_KEY, QString::number(powerSpectr.hz_high));
            objectPowerSpectr.insert(FFT_BIN_WIDTH_KEY, QString::number(powerSpectr.m_fft_bin_width));
            objectPowerSpectr.insert(NUM_SAMPLES_KEY, QString::number(powerSpectr.num_samples));

            QStringList list;
            for(int w=0; w<powerSpectr.m_power.count(); ++w)
                list.append(QString::number(static_cast<qreal>(powerSpectr.m_power.at(w))));

            objectPowerSpectr.insert(DATA_KEY, list.join(";"));

            array.append(objectPowerSpectr);
        }
        json_object.insert(POWERS_KEY, array);

        json_object.insert(ID_PARAMS_KEY, data->m_id_params);
    }

    const QJsonDocument doc(json_object);

    if(binary)
        return doc.toBinaryData();
    else
        return doc.toJson(QJsonDocument::Compact);
}
