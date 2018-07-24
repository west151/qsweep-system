#include "qsweepspectr.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>

static const QString DT_FORMAT = QStringLiteral("hh:mm:ss.zzz dd.MM.yyyy");

static const QString POWERS_KEY = QStringLiteral("powers");
static const QString FREQUENCY_MIN_KEY = QStringLiteral("frequency_min");
static const QString FREQUENCY_MAX_KEY = QStringLiteral("frequency_max");
static const QString FFT_BIN_WIDTH_KEY = QStringLiteral("fft_bin_width");
static const QString FFT_SIZE_KEY = QStringLiteral("fft_size");
static const QString DT_KEY = QStringLiteral("dt");
static const QString DATA_KEY = QStringLiteral("data");

class QSweepSpectrData : public QSharedData {
public:
    QSweepSpectrData(): QSharedData()
    {
        m_valid = false;
        m_powers.clear();
    }
    QSweepSpectrData(const QSweepSpectrData &other) : QSharedData(other)
    {
        m_valid = other.m_valid;
        m_powers = other.m_powers;
    }

    ~QSweepSpectrData() {}

    bool m_valid;
    QVector<PowerSpectr> m_powers;
};

QSweepSpectr::QSweepSpectr() : data(new QSweepSpectrData)
{
}

QSweepSpectr::QSweepSpectr(const QSweepSpectr &rhs) : data(rhs.data)
{
}

QSweepSpectr::QSweepSpectr(const QByteArray &json, const bool binary) : data(new QSweepSpectrData)
{
    QJsonDocument doc;
    if (binary)
        doc = QJsonDocument::fromBinaryData(json, QJsonDocument::BypassValidation);
    else
        doc = QJsonDocument::fromJson(json);

    QJsonObject jsonObject = doc.object();

    for(const QJsonValue &value: jsonObject.value(POWERS_KEY).toArray())
    {
        PowerSpectr powerSpectr;
        QJsonObject objectPowerSpectr = value.toObject();

        // DateTime
        auto dt = QDateTime::fromString(objectPowerSpectr.value(DT_KEY).toString(), DT_FORMAT);
        dt.setTimeSpec(Qt::UTC);
        powerSpectr.dateTime = dt;
        powerSpectr.m_fft_bin_width = objectPowerSpectr.value(FFT_BIN_WIDTH_KEY).toString().toDouble();
        powerSpectr.m_fft_size = objectPowerSpectr.value(FFT_SIZE_KEY).toString().toUInt();
        powerSpectr.m_frequency_min = objectPowerSpectr.value(FREQUENCY_MIN_KEY).toString().toULongLong();
        powerSpectr.m_frequency_max = objectPowerSpectr.value(FREQUENCY_MAX_KEY).toString().toULongLong();

        const QString valuePower = objectPowerSpectr.value(DATA_KEY).toString();
        QStringList listValue(valuePower.split(";", QString::SkipEmptyParts));

        for(const auto &strItem : listValue){
            const auto value = static_cast<qreal>(strItem.trimmed().toFloat());
            powerSpectr.m_power.append(value);
        }

        data->m_powers.append(powerSpectr);
    }

    if(!doc.isEmpty())
        data->m_valid = true;
    else
        data->m_valid = false;
}

QSweepSpectr &QSweepSpectr::operator=(const QSweepSpectr &rhs)
{
    if (this != &rhs) {
        data.operator=(rhs.data);
    }
    return *this;
}

QSweepSpectr::~QSweepSpectr()
{
}

bool QSweepSpectr::isValid() const
{
    return data->m_valid;
}

void QSweepSpectr::setPowerSpectr(const QVector<PowerSpectr> &value)
{
    data->m_powers = value;
}

QVector<PowerSpectr> QSweepSpectr::powerSpectr() const
{
    return data->m_powers;
}

QByteArray QSweepSpectr::exportToJson(const bool binary) const
{
    QJsonObject jsonObject;

    if(data->m_powers.count())
    {
        QJsonArray array;

        for(int i=0; i < data->m_powers.count(); ++i){
            QJsonObject objectPowerSpectr;
            const PowerSpectr powerSpectr = data->m_powers.at(i);

            objectPowerSpectr.insert(DT_KEY, powerSpectr.dateTime.toUTC().toString(DT_FORMAT));
            objectPowerSpectr.insert(FREQUENCY_MIN_KEY, QString::number(powerSpectr.m_frequency_min));
            objectPowerSpectr.insert(FREQUENCY_MAX_KEY, QString::number(powerSpectr.m_frequency_max));
            objectPowerSpectr.insert(FFT_BIN_WIDTH_KEY, QString::number(powerSpectr.m_fft_bin_width));
            objectPowerSpectr.insert(FFT_SIZE_KEY, QString::number(powerSpectr.m_fft_size));

            QStringList list;
            for(int w=0; w<powerSpectr.m_power.count(); ++w)
                list.append(QString::number(static_cast<qreal>(powerSpectr.m_power.at(w))));

            objectPowerSpectr.insert(DATA_KEY, list.join(";"));

            array.append(objectPowerSpectr);
        }
        jsonObject.insert(POWERS_KEY, array);
    }

    QJsonDocument doc(jsonObject);

    if(binary)
        return doc.toBinaryData();
    else
        return doc.toJson(QJsonDocument::Compact);
}
