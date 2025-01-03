#include "core_sweep.h"

#include <QCoreApplication>
#include <QDebug>

void sweep_message_output(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    }
}

void sweep_message_output_short(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s \n", localMsg.constData());
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info: %s \n", localMsg.constData());
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s \n", localMsg.constData());
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s \n", localMsg.constData());
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s \n", localMsg.constData());
        break;
    }
}

int main(int argc, char *argv[])
{
// #ifdef QT_DEBUG
// //    qInstallMessageHandler(sweep_message_output);
//     qInstallMessageHandler(sweep_message_output_short);
// #endif

    QCoreApplication app(argc, argv);
    QString cfgDir;

#ifdef Q_OS_WIN64
    cfgDir = app.applicationDirPath();
#else
    cfgDir = ("/etc/" + app.applicationName());const QDir pluginDir {"plugins"};
#endif

    qDebug() << "Config dir:" << cfgDir;

    core_sweep core_sweep_server(cfgDir, app.applicationName());
    /* 1 */
    bool result = core_sweep_server.read_settings();
    /* 2 */
    core_sweep_server.initialization();
    /* 3 */
    core_sweep_server.start_server();

    return app.exec();
}
