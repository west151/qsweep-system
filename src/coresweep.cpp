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

    // RX LNA (IF) gain, 0-40dB, 8dB steps
    // RX VGA (baseband) gain, 0-62dB, 2dB steps
    ptrSweepWorker->runSweepWorker(2400, 2700, 500000, 40, 62);

    return app.exec();
}
