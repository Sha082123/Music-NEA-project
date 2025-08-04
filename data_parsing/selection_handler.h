#ifndef SELECTION_HANDLER_H
#define SELECTION_HANDLER_H

#include <QObject>
#include "xml_parser.h"

class selection_handler : public QObject
{
    Q_OBJECT
public:
    explicit selection_handler(QObject *parent = nullptr);

    Q_INVOKABLE QString get_element_at_coords(const int page_number, const double &x, const double &y, const QString &file_name);
    Q_INVOKABLE void parse_page_xml(const int page_number);

    void add_svg_data (const std::string &svg_data);
    void clear_svg_data();
    std::string get_svg_data(const int &page_number);


signals:

private:
    std::vector<std::string> list_svg_data; // List to store SVG data
    xml_parser* xml_hit_detection;

};

#endif // SELECTION_HANDLER_H
