#ifndef LNA_GAIN_DATA_H
#define LNA_GAIN_DATA_H

#include <QSharedData>
#include <QMetaType>

class lna_gain_data;

class lna_gain
{
public:
    lna_gain();
    lna_gain(const lna_gain &);
    lna_gain &operator=(const lna_gain &);
    ~lna_gain();

    void set_title(const QString &);
    QString title()const;

    void set_lna_gain_value(const quint32 &);
    quint32 lna_gain_value()const;

private:
    QSharedDataPointer<lna_gain_data> data;
};

Q_DECLARE_METATYPE(lna_gain)

#endif // LNA_GAIN_DATA_H
