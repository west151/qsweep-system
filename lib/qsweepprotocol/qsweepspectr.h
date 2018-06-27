#ifndef QSWEEPSPECTR_H
#define QSWEEPSPECTR_H

#include <QtCore/qshareddata.h>
#include <QtCore/qmetatype.h>

#include "qsweepprotocol_global.h"

class QSweepSpectrData;

class QSWEEPPROTOCOLSHARED_EXPORT QSweepSpectr
{
public:
    QSweepSpectr();
    QSweepSpectr(const QSweepSpectr &);
    QSweepSpectr(const QByteArray &json, const bool binary = false);
    QSweepSpectr &operator=(const QSweepSpectr &);
    ~QSweepSpectr();

    bool isValid() const;

    QByteArray exportToJson(const bool binary = false) const;

private:
    QSharedDataPointer<QSweepSpectrData> data;
};

Q_DECLARE_METATYPE(QSweepSpectr)

#endif // QSWEEPSPECTR_H
