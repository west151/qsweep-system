#include "ctrlsweepworker.h"

#ifdef QT_DEBUG
#include <QtCore/qdebug.h>
#endif

CtrlSweepWorker::CtrlSweepWorker(QObject *parent) : QObject(parent)
{

}

void CtrlSweepWorker::startSweepWorkerTest()
{
    emit sendRunSweepWorker();
}
