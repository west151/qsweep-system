#include "coresweep.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("CoreSweepServer");
    QCoreApplication::setApplicationVersion("1.0");

    QCoreApplication app(argc, argv);
    CoreSweep coreSweepServer(app.applicationFilePath());

    return app.exec();
}
