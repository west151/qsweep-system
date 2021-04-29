#ifndef FFT_WIDT_H
#define FFT_WIDT_H

#include <QSharedDataPointer>

class fft_width_data;

class fft_width
{
public:
    fft_width();
    fft_width(const fft_width &);
    fft_width &operator=(const fft_width &);
    ~fft_width();

    void set_title(const QString &);
    QString title()const;

    void set_fft_width_value(const quint32 &);
    quint32 fft_width_value()const;

private:
    QSharedDataPointer<fft_width_data> data;
};

#endif // FFT_WIDT_H
