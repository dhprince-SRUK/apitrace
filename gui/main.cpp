#include <stdlib.h>

#include "mainwindow.h"

#include "apitrace.h"
#include "apitracecall.h"

#include "os_string.hpp"
#include "os_process.hpp"

#include <QApplication>
#include <QMetaType>
#include <QVariant>
#include <QImage>

Q_DECLARE_METATYPE(QList<ApiTraceFrame*>);
Q_DECLARE_METATYPE(QVector<ApiTraceCall*>);
Q_DECLARE_METATYPE(Qt::CaseSensitivity);
Q_DECLARE_METATYPE(ApiTrace::SearchResult);
Q_DECLARE_METATYPE(ApiTrace::SearchRequest);
Q_DECLARE_METATYPE(QList<QImage>);

static void usage(void)
{
    qWarning("usage: qapitrace [options] [TRACE] [CALLNO]\n"
             "Valid options include:\n"
             "    -h, --help            Print this help message\n"
             "    --remote-target HOST  Replay trace on remote target HOST\n"
             "                          --remote-target root@192.168.x.y\n"
             "    --remote-env ENVVAR   Environment variables to send to remote target HOST\n"
             "                          e.g. --remote-env 'DISPLAY=\":0.0\" LD_LIBRARY_PATH=\"/usr/lib\"'\n");
}

int main(int argc, char **argv)
{
    QApplication::setGraphicsSystem("raster");
    QApplication app(argc, argv);

    qRegisterMetaType<QList<ApiTraceFrame*> >();
    qRegisterMetaType<QVector<ApiTraceCall*> >();
    qRegisterMetaType<ApiTraceState>();
    qRegisterMetaType<Qt::CaseSensitivity>();
    qRegisterMetaType<ApiTrace::SearchResult>();
    qRegisterMetaType<ApiTrace::SearchRequest>();
    qRegisterMetaType<QList<QImage> >();

#ifndef Q_OS_WIN
    os::String currentProcess = os::getProcessName();
    currentProcess.trimFilename();
    QString path = qgetenv("PATH");
    path = QLatin1String(currentProcess.str()) + QLatin1String(":") + path;
    qputenv("PATH", path.toLatin1());
#endif

    QStringList args = app.arguments();
    QString remoteTarget;
    QString remoteEnv;

    int i = 1;
    while (i < args.count()) {
        QString arg = args[i];
        if (arg[0] != QLatin1Char('-')) {
            break;
        }
        ++i;
//        qDebug("arg[%d]=%s", i, argv[i]);
        if (arg == QLatin1String("--")) {
            break;
        } else if (arg == QLatin1String("--remote-target")) {
            if (i == args.count()) {
                qWarning("Option --remote-target requires an argument.\n");
                exit(1);
            }
            remoteTarget = args[i];
            ++i;
        } else if (arg == QLatin1String("--remote-env")) {
            if (i == args.count()) {
                qWarning("Option --remote-env requires an argument.\ne.g. \'DISPLAY=\"0:.0\" LD_PRELOAD=\"path-to-some-lib.so\"\'");
                exit(1);
            }
            remoteEnv = args[i];
            ++i;
        } else if (arg == QLatin1String("-h") ||
                   arg == QLatin1String("--help")) {
            usage();
            exit(0);
        } else {
            usage();
            exit(1);
        }
    }

    MainWindow window;
    window.show();

    if (i < args.count()) {
        QString fileName = args[i++];

        int callNum = -1;
        if (i < args.count()) {
            callNum = args[i++].toInt();
        }
        window.loadTrace(fileName, callNum);
    }

    if (remoteTarget.length()) {
        window.setRemoteTarget(remoteTarget);
    }

    if (remoteEnv.length()) {
        window.setRemoteEnv(remoteEnv);
    }
    app.exec();
}
