#ifndef QSWEEPPROTOCOL_GLOBAL_H
#define QSWEEPPROTOCOL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QSWEEPPROTOCOL_LIBRARY)
#  define QSWEEPPROTOCOLSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QSWEEPPROTOCOLSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QSWEEPPROTOCOL_GLOBAL_H
