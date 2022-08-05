#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "frame.h"
#include "calibrate.h"
#include "marker.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<Frame>("com.app.frame", 1, 0, "Frame");
    qmlRegisterType<Calibrate>("com.app.calibrate", 1, 0, "Calibrate");
    qmlRegisterType<Marker>("com.app.marker", 1, 0, "Marker");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
