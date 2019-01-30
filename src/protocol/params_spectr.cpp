#include "params_spectr.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QUuid>

#include "constkeys.h"

class params_spectr_data : public QSharedData {
public:
    params_spectr_data(): QSharedData()
    {
        m_valid = false;
        m_one_shot = true;
        m_start_spectr = false;
        m_frequency_min = 30;
        m_frequency_max = 6000;
        m_fft_bin_width = 500000;
        m_lna_gain = 0;
        m_vga_gain = 0;
        m_id = QUuid::createUuid().toString().mid(1, 8);
    }
    params_spectr_data(const params_spectr_data &other) : QSharedData(other)
    {
        m_valid = other.m_valid;
        m_one_shot = other.m_one_shot;
        m_start_spectr = other.m_start_spectr;
        m_id = other.m_id;
        m_frequency_min = other.m_frequency_min;
        m_frequency_max = other.m_frequency_max;
        m_fft_bin_width = other.m_fft_bin_width;
        m_lna_gain = other.m_lna_gain;
        m_vga_gain = other.m_vga_gain;
        m_descr = other.m_descr;
    }

    ~params_spectr_data() {}

    bool m_valid;
    bool m_one_shot;
    bool m_start_spectr;
    QString m_id;
    quint32 m_lna_gain;    // RX LNA (IF) gain, 0-40dB, 8dB steps
    quint32 m_vga_gain;    // RX VGA (baseband) gain, 0-62dB, 2dB steps
    quint32 m_fft_bin_width;   // FFT bin width (frequency resolution) in Hz\n")
    quint32 m_frequency_min;   // frequency min MHz
    quint32 m_frequency_max;   // frequency max MHz
    QString m_descr;
};

params_spectr::params_spectr() : data(new params_spectr_data)
{
}

params_spectr::params_spectr(const params_spectr &rhs) : data(rhs.data)
{
}

params_spectr::params_spectr(const QByteArray &json, const bool binary) : data(new params_spectr_data)
{
    QJsonDocument doc;
    if (binary)
        doc = QJsonDocument::fromBinaryData(json, QJsonDocument::BypassValidation);
    else
        doc = QJsonDocument::fromJson(json);

    const QJsonObject json_object(doc.object());
    bool ok;
    data->m_id = json_object.value(ID_KEY).toString();
    data->m_frequency_min = json_object.value(FREQUENCY_MIN_KEY).toString().toUInt(&ok, 10);
    data->m_frequency_max = json_object.value(FREQUENCY_MAX_KEY).toString().toUInt();
    data->m_fft_bin_width = json_object.value(FFT_BIN_WIDTH_KEY).toString().toUInt();
    data->m_lna_gain = json_object.value(LNA_GAIN_KEY).toString().toUInt();
    data->m_vga_gain = json_object.value(VGA_GAIN_KEY).toString().toUInt();
    data->m_one_shot = json_object.value(ONE_SHOT_KEY).toBool();
    data->m_start_spectr = json_object.value(START_SPECTR_KEY).toBool();
    data->m_descr = json_object.value(DESCR_KEY).toString();    

    if(!doc.isEmpty())
        data->m_valid = true;
    else
        data->m_valid = false;
}

params_spectr &params_spectr::operator=(const params_spectr &rhs)
{
    if (this != &rhs) {
        data.operator=(rhs.data);
    }
    return *this;
}

params_spectr::~params_spectr()
{
}

bool params_spectr::is_valid() const
{
    return data->m_valid;
}

QString params_spectr::id_params_spectr() const
{
    return data->m_id;
}

void params_spectr::set_one_shot(const bool &value)
{
    data->m_one_shot = value;
}

bool params_spectr::one_shot() const
{
    return data->m_one_shot;
}

void params_spectr::set_start_spectr(const bool &value)
{
    data->m_start_spectr = value;
}

bool params_spectr::start_spectr() const
{
    return data->m_start_spectr;
}

void params_spectr::set_lna_gain(const quint32 &value)
{
    data->m_lna_gain = value;
}

quint32 params_spectr::lna_gain() const
{
    return data->m_lna_gain;
}

void params_spectr::set_vga_gain(const quint32 &value)
{
    data->m_vga_gain = value;
}

quint32 params_spectr::vga_gain() const
{
    return data->m_vga_gain;
}

void params_spectr::set_fft_bin_width(const quint32 &value)
{
    data->m_fft_bin_width = value;
}

quint32 params_spectr::fft_bin_width() const
{
    return data->m_fft_bin_width;
}

void params_spectr::set_frequency_min(const quint32 &value)
{
    data->m_frequency_min = value;
}

quint32 params_spectr::frequency_min() const
{
    return data->m_frequency_min;
}

void params_spectr::set_frequency_max(const quint32 &value)
{
    data->m_frequency_max = value;
}

quint32 params_spectr::frequency_max() const
{
    return data->m_frequency_max;
}

void params_spectr::set_descr(const QString &value)
{
    data->m_descr = value;
}

QString params_spectr::descr() const
{
    return data->m_descr;
}

QByteArray params_spectr::export_json(const bool binary) const
{
    QJsonObject json_object;

    json_object.insert(ID_KEY, data->m_id);
    json_object.insert(FREQUENCY_MIN_KEY, QString::number(data->m_frequency_min));
    json_object.insert(FREQUENCY_MAX_KEY, QString::number(data->m_frequency_max));
    json_object.insert(FFT_BIN_WIDTH_KEY, QString::number(data->m_fft_bin_width));
    json_object.insert(LNA_GAIN_KEY, QString::number(data->m_lna_gain));
    json_object.insert(VGA_GAIN_KEY, QString::number(data->m_vga_gain));
    json_object.insert(ONE_SHOT_KEY, data->m_one_shot); 
    json_object.insert(START_SPECTR_KEY, data->m_start_spectr);
    json_object.insert(DESCR_KEY, data->m_descr);    

    const QJsonDocument doc(json_object);

    if(binary)
        return doc.toBinaryData();
    else
        return doc.toJson(QJsonDocument::Compact);
}
