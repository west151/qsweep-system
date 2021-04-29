#include "vga_gain.h"

class vga_gain_data : public QSharedData {
public:
    vga_gain_data(): QSharedData()
    {
    }
    vga_gain_data(const vga_gain_data &other) : QSharedData(other)
    {
        m_title = other.m_title;
        m_vga_gain = other.m_vga_gain;
    }

    ~vga_gain_data() {}

    QString m_title;
    quint32 m_vga_gain;
};

vga_gain::vga_gain() : data(new vga_gain_data)
{
}

vga_gain::vga_gain(const vga_gain &rhs) : data(rhs.data)
{
}

vga_gain &vga_gain::operator=(const vga_gain &rhs)
{
    if (this != &rhs) {
        data.operator=(rhs.data);
    }
    return *this;
}

vga_gain::~vga_gain()
{
}

void vga_gain::set_title(const QString &value)
{
    data->m_title = value;
}

QString vga_gain::title() const
{
    return data->m_title;
}

void vga_gain::set_vga_gain_value(const quint32 &value)
{
    data->m_vga_gain = value;
}

quint32 vga_gain::vga_gain_value() const
{
    return data->m_vga_gain;
}
