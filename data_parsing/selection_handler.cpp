#include "selection_handler.h"
#include <QDebug>
#include "globals.h"

selection_handler::selection_handler(QObject *parent)
    : QObject{parent}
{
    xml_hit_detection = new xml_parser(this); // Initialize the XML parser
}

QString selection_handler::get_element_at_coords(const int page_number, const double &x, const double &y, const QString &file_name)
{


    //qInfo() << list_svg_data[0];
    if (page_number < 0 || page_number > list_svg_data.size()) {
        qInfo() << "did not work";
        return QString(); // Return an empty string if the page number is out of bounds
    }


    // QString id = coords_converter->element_on_coords (svg_data, x, y);

    //-----------------------------------------QDomDocument Method------------------------------------------------//

    const std::string &svg_data = list_svg_data[page_number-1]; // Get the SVG data for the specified page
    //g_xml_parser->parse_xml(QString::fromStdString (svg_data));

    QString id = g_xml_parser->element_from_point(QPointF(x, y), page_number); // Get the element at the specified coordinates

    //file_name is image://image_provider/myFolder/file, need to make it so its myFolder/file

    QString string = file_name;
    QString path = string.replace("image://image_provider/", "");

    g_resvg_loader->set_attribute(id.toStdString(), "fill", "red", path, svg_data);
    // turn selected into red colour

    return id;
}

void selection_handler::parse_page_xml(const int page_number)
{
    g_xml_parser->parse_xml(QString::fromStdString (list_svg_data[page_number-1]));
}

void selection_handler::add_svg_data(const std::string &svg_data)
{
    list_svg_data.push_back(svg_data);
}

void selection_handler::clear_svg_data()
{
    list_svg_data.clear(); // Clear the list of SVG data
}


