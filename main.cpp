#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QApplication>
#include <QQuickView>
#include <QQmlContext>

#include "appcore.h"
#include "controller.h"

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
//    AppCore appCore;
    Controller controller;
    QQmlApplicationEngine engine;
    QQmlContext* context = engine.rootContext();
//    context->setContextProperty("backend", &appCore);
    context->setContextProperty("backend", &controller);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }
    int rc = app.exec();
    return rc;
}
