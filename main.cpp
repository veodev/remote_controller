#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QApplication>

#include "appcore.h"

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    qmlRegisterType<AppCore>("remote_controller.appcore", 1, 0, "AppCore");
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
//    QApplication app(argc, argv);
//    UiOnWidget* uiOnWidget = new UiOnWidget;
//    uiOnWidget->show();


    return app.exec();
}
