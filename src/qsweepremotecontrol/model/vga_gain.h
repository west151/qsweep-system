#ifndef VGA_GAIN_H
#define VGA_GAIN_H

#include <QSharedData>
#include <QMetaType>

class vga_gain_data;

class vga_gain
{
public:
    vga_gain();
    vga_gain(const vga_gain &);
    vga_gain &operator=(const vga_gain &);
    ~vga_gain();

    void set_title(const QString &);
    QString title()const;

    void set_vga_gain_value(const quint32 &);
    quint32 vga_gain_value()const;

private:
    QSharedDataPointer<vga_gain_data> data;
};

Q_DECLARE_METATYPE(vga_gain)

#endif // VGA_GAIN_H
