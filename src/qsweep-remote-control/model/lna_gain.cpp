#include "lna_gain.h"

class lna_gain_data : public QSharedData {
public:
    lna_gain_data(): QSharedData()
    {
    }
    lna_gain_data(const lna_gain_data &other) : QSharedData(other)
    {
        m_title = other.m_title;
        m_lna_gain = other.m_lna_gain;
    }

    ~lna_gain_data() {}

    QString m_title;
    quint32 m_lna_gain;
};

lna_gain::lna_gain() : data(new lna_gain_data)
{
}

lna_gain::lna_gain(const lna_gain &rhs) : data(rhs.data)
{
}

lna_gain &lna_gain::operator=(const lna_gain &rhs)
{
    if (this != &rhs) {
        data.operator=(rhs.data);
    }
    return *this;
}

lna_gain::~lna_gain()
{
}

void lna_gain::set_title(const QString &value)
{
    data->m_title = value;
}

QString lna_gain::title() const
{
    return data->m_title;
}

void lna_gain::set_lna_gain_value(const quint32 &value)
{
    data->m_lna_gain = value;
}

quint32 lna_gain::lna_gain_value() const
{
    return data->m_lna_gain;
}
