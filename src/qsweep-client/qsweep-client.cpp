#include "core_sweep_client.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSettings>
#include <QFileInfo>

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
#ifdef QT_DEBUG
//    qInstallMessageHandler(sweep_message_output);
    qInstallMessageHandler(sweep_message_output_short);
#endif

    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    // QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setOrganizationName("qsweep");
    QGuiApplication::setApplicationName("qsweep-client");
    QGuiApplication::setApplicationVersion("1.0");

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine* engine = new QQmlApplicationEngine;
    QObject::connect(engine, &QQmlApplicationEngine::quit,
                     QGuiApplication::instance(), &QGuiApplication::quit);
    // settings
    QSettings cfg(QSettings::IniFormat, QSettings::UserScope,
                  app.organizationName(), app.applicationName());
    QString config_dir = QFileInfo(cfg.fileName()).absolutePath() + "/";

    core_sweep_client main_sweep_client(config_dir, app.applicationName());
    /* 1 */
    bool result = main_sweep_client.read_settings();
    /* 2 */
    main_sweep_client.initialization(engine);
    /* 3 */
    main_sweep_client.start_client();

    return app.exec();
}
