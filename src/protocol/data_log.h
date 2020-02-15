#ifndef DATA_LOG_H
#define DATA_LOG_H

#include <QSharedData>
#include <QMetaType>

class data_log_data;

class data_log
{
public:
    data_log();
    data_log(const data_log &);
    data_log(const QByteArray &json);
    data_log &operator=(const data_log &);
    ~data_log();

    bool is_valid() const;

    QString text_message()const;
    void set_text_message(const QString &);

    QDateTime date_time()const;

    QByteArray to_json() const;

private:
    QSharedDataPointer<data_log_data> data;
};

Q_DECLARE_METATYPE(data_log)

#endif // DATA_LOG_H
