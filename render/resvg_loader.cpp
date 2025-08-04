#include "resvg_loader.h"
#include "globals.h"
#include <lunasvg.h>

resvg_loader::resvg_loader(QObject *parent)
    : QObject{parent}
{
    QDir dir(QDir::currentPath());
    dir.cdUp();
    dir.cdUp();
    root = dir.path();
    font_path = dir.path() + "/external/verovio/fonts/"; // Path to the Verovio fonts directory

    // renderer = new ResvgRenderer(); // Initialize the Resvg loader instance
    // options = new ResvgOptions();

    // options->loadSystemFonts ();

    // options->loadFontFile (font_path+"Leipzig/Leipzig.ttf");
    // options->loadFontFile (font_path+"Leland/Leland.otf");
    // options->loadFontFile (font_path+"Gootville/Gootville.otf");
    // options->loadFontFile (font_path+"Bravura/Bravura.otf");
    // options->loadFontFile (font_path+"Petaluma/Petaluma.otf");

    std::string Leipzig = font_path.toStdString()+"Leipzig/Leipzig.ttf";
    const char* Leipzig_cstr = Leipzig.c_str();

    std::string Leland = font_path.toStdString()+"Leland/Leland.otf";
    const char* Leland_cstr = Leland.c_str();

    std::string Gootville = font_path.toStdString()+"Leland/Leland.otf";
    const char* Gootville_cstr = Gootville.c_str();

    std::string Bravura = font_path.toStdString()+"Leland/Leland.otf";
    const char* Bravura_cstr = Bravura.c_str();

    std::string Petaluma = font_path.toStdString()+"Leland/Leland.otf";
    const char* Petaluma_cstr = Petaluma.c_str();

    lunasvg_add_font_face_from_file("Leipzig", false, false, Leipzig_cstr);
    lunasvg_add_font_face_from_file("Leland", false, false, Leland_cstr);
    lunasvg_add_font_face_from_file("Gootville", false, false, Gootville_cstr);
    lunasvg_add_font_face_from_file("Bravura", false, false, Bravura_cstr);
    lunasvg_add_font_face_from_file("Petaluma", false, false, Petaluma_cstr);

}

QImage resvg_loader::render(const std::string &svg_string)
{
    // QByteArray byte_array = QByteArray::fromStdString(svg_string);
    // renderer->load (byte_array, *options); // load the svg string

    // QSize size = renderer->defaultSize(); // Get the default size of the SVG
    // QImage image = renderer->renderToImage(size); // Render the SVG to an image

    document = lunasvg::Document::loadFromData(svg_string.data());

    auto bitmap = document->renderToBitmap();

    QImage img(bitmap.data(), bitmap.width(), bitmap.height(), QImage::Format_ARGB32);

    QImage image = img.copy();

    // qInfo() << "SVG rendered to image with size:" << image.size();

    int page_height = bitmap.height();

    add_page_height(page_height);


    return image; // Return the rendered image
}

void resvg_loader::add_to_image_provider(const std::string &data, const QString &file_name)
{
    QImage image = g_resvg_loader->render(data);

    QImage blank_image(image.width(), image.height(), QImage::Format_ARGB32);
    blank_image.fill(Qt::white);

    g_image_provider->addImage(file_name + "/reloader", blank_image);

    //qInfo() <<  Qt::endl << Qt::endl << Qt::endl << image << Qt::endl << Qt::endl << Qt::endl;

    g_image_provider->addImage(file_name, image); // Add the image to the global image provider
}

void resvg_loader::set_attribute(std::string element_id, const std::string &name, const std::string &value,
                                 const QString &file_name, const std::string &file_data)
{
    document = lunasvg::Document::loadFromData(file_data.data());

    auto element = document->getElementById(element_id);
    element.setAttribute(name, value);

    //qInfo() << "element has class attribute:" << element.hasAttribute("class");
    //qInfo() << "element id : " << QString::fromStdString(element_id);

    auto bitmap = document->renderToBitmap();

    //document->renderToBitmap().writeToPng("C:/Users/david/Downloads/test 1/test.png");

    QImage img(bitmap.data(), bitmap.width(), bitmap.height(), QImage::Format_ARGB32);

    QImage image = img.copy();

    g_image_provider->addImage(file_name, image);
}

void resvg_loader::add_page_height(int &height)
{
    if (page_heights.empty()) {
        page_heights.append(height);
    }
    else {
        int previous_height = page_heights.last();
        page_heights.append(previous_height + height);
    }
}

int resvg_loader::get_page_height(int &page_number)
{
    if (page_number == 0) {
        return 0; // Return 0 for page 0, as it does not exist
    }
    return page_heights[page_number - 1];
}
