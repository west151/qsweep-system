#include "qsweepspectr.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

class QSweepSpectrData : public QSharedData {
public:
    QSweepSpectrData(): QSharedData()
    {
        m_valid = false;
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
//    data->m_textMessage = jsonObject.value(TEXT_MESSAGE_KEY).toString();

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

QByteArray QSweepSpectr::exportToJson(const bool binary) const
{
    QJsonObject jsonObject;
//    jsonObject.insert(TEXT_MESSAGE_KEY, data->m_textMessage);

    QJsonDocument doc(jsonObject);

    if(binary)
        return doc.toBinaryData();
    else
        return doc.toJson(QJsonDocument::Compact);
}
