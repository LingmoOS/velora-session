#include "splashwidget.h"

#include <QApplication>
#include <QDBusServiceWatcher>
#include <QDBusConnection>
#include <QTimer>

static const char *kDockService = "org.deepin.dde.Dock1";

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("dde-splash");

    SplashWidget splash;
    splash.show();

    // Watch for the dock service - when it appears, desktop is ready
    QDBusServiceWatcher *watcher = new QDBusServiceWatcher(kDockService,
                                                           QDBusConnection::sessionBus(),
                                                           QDBusServiceWatcher::WatchForRegistration,
                                                           &splash);

    QObject::connect(watcher, &QDBusServiceWatcher::serviceRegistered, &splash, [&splash]() {
        splash.fadeOutAndQuit();
    });

    // Safety timeout: exit after 30s even if dock never comes
    QTimer::singleShot(30000, &splash, [&splash]() {
        splash.fadeOutAndQuit();
    });

    return app.exec();
}
