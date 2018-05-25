#include "coresweep.h"
#include <QCoreApplication>

#include "src/hackrfinfo.h"

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

CoreSweep::CoreSweep(QObject *parent) : QObject(parent),
    ptrHackrfInfo(new HackrfInfo(this))
{

}

int CoreSweep::runCoreSweep(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    ptrHackrfInfo->getHackrfInfo();

    return app.exec();
}
