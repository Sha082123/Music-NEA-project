#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>
#include <QQuickView>
#include "file_opening/file_open.h"
#include "render/render_file.h"
#include "file_opening/scan_existing.h"
#include "globals.h"
#include "svgpp/svgpp.hpp"
#include "data_parsing/xml_parser.h"
#include "data_parsing/parser_data.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    g_image_provider = new image_provider(); // Initialize the global image provider
    g_verovio_loader = new verovio_loader(); // Initialize the global Verovio loader
    g_resvg_loader = new resvg_loader(); // Initialize the global Resvg loader
    g_xml_parser = new xml_parser(); // Initialize the global XML parser
    g_mei_parser = new mei_parser(); // Initialize the global MEI parser
    g_parser_data = new parser_data(); // Initialize the global parser data


    engine.addImageProvider("image_provider", g_image_provider);
    engine.load(QUrl::fromLocalFile("main.qml"));

    engine.rootContext ()->setContextProperty("file_open", new file_open());
    engine.rootContext()->setContextProperty("render", new render_file(&engine));
    engine.rootContext()->setContextProperty("scan_existing", new scan_existing(&engine));
    engine.rootContext()->setContextProperty ("xml_parser", g_xml_parser);
    engine.rootContext()->setContextProperty ("mei_parser", g_mei_parser);
    engine.rootContext()->setContextProperty ("parser_data", g_parser_data);

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
