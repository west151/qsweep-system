#include "fft_width.h"

class fft_width_data : public QSharedData
{
public:
    fft_width_data(): QSharedData()
    {
    }
    fft_width_data(const fft_width_data &other) : QSharedData(other)
    {
        m_title = other.m_title;
        m_fft_width = other.m_fft_width;
    }

    ~fft_width_data() {}

    QString m_title;
    quint32 m_fft_width;
};

fft_width::fft_width() : data(new fft_width_data)
{

}

fft_width::fft_width(const fft_width &rhs) : data(rhs.data)
{

}

fft_width &fft_width::operator=(const fft_width &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

fft_width::~fft_width()
{

}

void fft_width::set_title(const QString &value)
{
    data->m_title = value;
}

QString fft_width::title() const
{
    return data->m_title;
}

void fft_width::set_fft_width_value(const quint32 &value)
{
    data->m_fft_width = value;
}

quint32 fft_width::fft_width_value() const
{
    return data->m_fft_width;
}
