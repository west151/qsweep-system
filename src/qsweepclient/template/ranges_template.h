#ifndef RANGES_TEMPLATE_H
#define RANGES_TEMPLATE_H

#include <QtCore/qshareddata.h>
#include <QtCore/qmetatype.h>

class ranges_template_data;

class ranges_template
{
public:
    ranges_template();
    ranges_template(const ranges_template &);
    ranges_template(const QByteArray &json, const bool binary = false);
    ranges_template &operator=(const ranges_template &);
    ~ranges_template();

    bool is_valid() const;

    void set_name_ranges(const QString &);
    QString name_ranges()const;

    void set_descr_ranges(const QString &);
    QString descr_ranges()const;

    void set_freq_ranges(const QList<QPair<quint64, quint64> > &);
    const QList<QPair<quint64, quint64> > freq_ranges()const;

private:
    QSharedDataPointer<ranges_template_data> data;
};

Q_DECLARE_METATYPE(ranges_template)

#endif // RANGES_TEMPLATE_H
