#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "toolkit.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;


    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Music_NEA_MSVC", "Main");

    return app.exec();
}
