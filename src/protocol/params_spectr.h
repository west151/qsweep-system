#ifndef PARAMS_SPECTR_H
#define PARAMS_SPECTR_H

#include <QSharedData>
#include <QMetaType>

class params_spectr_data;

class params_spectr
{
public:
    params_spectr();
    params_spectr(const params_spectr &);
    params_spectr(const QByteArray &json, const bool binary = false);
    params_spectr &operator=(const params_spectr &);
    ~params_spectr();

    bool is_valid() const;

    void set_id_params(const QString &);
    QString id_params()const;

    void set_one_shot(const bool &);
    bool one_shot()const;

    void set_start_spectr(const bool &);
    bool start_spectr()const;

    void set_lna_gain(const quint32 &);
    quint32 lna_gain()const;

    void set_vga_gain(const quint32 &);
    quint32 vga_gain()const;

    void set_fft_bin_width(const quint32 &);
    quint32 fft_bin_width()const;

    void set_frequency_min(const quint32 &);
    quint32 frequency_min()const;

    void set_frequency_max(const quint32 &);
    quint32 frequency_max()const;

    void set_descr(const QString &);
    QString descr()const;

    QByteArray to_json(const bool binary = false) const;

private:
    QSharedDataPointer<params_spectr_data> data;
};

Q_DECLARE_METATYPE(params_spectr)

#endif // PARAMS_SPECTR_H
