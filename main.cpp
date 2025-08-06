#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>
#include <QQuickView>
#include "file_opening/file_open.h"
#include "render/render_file.h"
#include "render/image_provider.h"
#include "file_opening/scan_existing.h"
#include "part_objects/part_object.h"
#include "part_objects/part_manager.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    g_image_provider = new image_provider(); // Initialize the global image provider
    g_verovio_loader = new verovio_loader(&engine); // Initialize the global Verovio loader

    //part_object *main_score_part = new part_object(&engine, "main_score");
    part_manager *part_manager_instance = new part_manager(&engine, &engine);

    engine.addImageProvider("image_provider", g_image_provider);
    engine.load(QUrl::fromLocalFile("main.qml"));

    engine.rootContext ()->setContextProperty("file_open", new file_open(nullptr, g_verovio_loader));
    engine.rootContext()->setContextProperty("scan_existing", new scan_existing(&engine));


    engine.rootContext()->setContextProperty("part_manager", part_manager_instance);
    //engine.rootContext()->setContextProperty("main_score", main_score_part);


    scan_existing scanner = scan_existing(&engine);
    scanner.scan_directory(QDir::currentPath () + "/UserFiles/project_files/");

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Music_NEA_MSVC", "Main");

    return app.exec();
}
