#include "coresweep.h"
#include <QCoreApplication>

#include <time.h>

#include "src/hackrfinfo.h"
#include "src/sweepworker.h"

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

CoreSweep::CoreSweep(QObject *parent) : QObject(parent),
    ptrHackrfInfo(new HackrfInfo(this)),
    ptrSweepWorker(new SweepWorker(this))
{

}

int CoreSweep::runCoreSweep(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    //ptrHackrfInfo->getHackrfInfo();

    ptrSweepWorker->runSweepWorker();

    return app.exec();
}
