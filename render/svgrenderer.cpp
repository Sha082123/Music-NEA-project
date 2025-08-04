#include "svgrenderer.h"
#include "lunasvg.h"

using namespace lunasvg;

SvgRenderer::SvgRenderer(QObject *parent)
    : QObject{parent}
{
 // Initialize the image provider from the global instance;
}

// void SvgRenderer::Render_svg(const QString &file_path, const QString &output_path)
// {
//     auto svg_file = Document::loadFromFile(file_path.toStdString());
//     auto bitmap = svg_file->renderToBitmap();
//     bitmap.writeToPng(output_path.toStdString ());
//     qInfo() << "SVG rendered to PNG:" << output_path << Qt::endl; // Log to console that the SVG has been rendered
// }

void SvgRenderer::add_to_image_provider(const std::string &data, const QString &filename)
{

    QImage image = g_resvg_loader->render(data);

    qInfo() <<  Qt::endl << Qt::endl << Qt::endl << image << Qt::endl << Qt::endl << Qt::endl;

    g_image_provider->addImage(filename, image); // Add the image to the global image provider

}

