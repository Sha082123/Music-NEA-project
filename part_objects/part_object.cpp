#include "part_object.h"
#include <QDebug>

part_object::part_object(QObject *parent, QString part_name)
    : QObject{parent}
{
    //m_image_provider = new image_provider(); // Initialize the global image provider
    m_verovio_loader = new verovio_loader(this); // Initialize the global Verovio loader
    m_resvg_loader = new resvg_loader(this); // Initialize the global Resvg loader
    m_xml_parser = new xml_parser(this); // Initialize the global XML parser
    m_mei_parser = new mei_parser(this, m_xml_parser, nullptr); // Initialize the global MEI parser
    m_parser_data = new parser_data(this, m_xml_parser, m_mei_parser); // Initialize the global parser data

    m_mei_parser->set_parser_data(m_parser_data); // set the parser data for use

    qInfo() << "checkpoint1";

    m_render_file = new render_file(this, m_mei_parser, m_xml_parser,
                                    m_parser_data, m_verovio_loader, m_resvg_loader);

    qInfo() << "checkpoint2";


    m_part_name = part_name;


}


void part_object::openFile(const QString &file_path, int mode)
{
    setfile_path(file_path);

    m_render_file->openFile(file_path, mode); // Open the file using the render file object
    setlist_PNG_paths (m_render_file->list_PNG_paths ());

    setReh_y_coords (m_parser_data->reh_y_coords ());
    setBreak_list (m_parser_data->break_list ());

    // for (const QString &path : m_list_PNG_paths) {
    //     qInfo() << "PNG Path: " << path; // Debugging output to check the paths
    // }
}

void part_object::update()
{
    m_render_file->update(); // Update the render file object
    setlist_PNG_paths (m_render_file->list_PNG_paths ());

    setReh_y_coords (m_parser_data->reh_y_coords ());
    setBreak_list (m_parser_data->break_list ());

    // for (const QString &path : m_list_PNG_paths) {
    //     qInfo() << "PNG Path: " << path; // Debugging output to check the paths
    // }
}

int part_object::new_break_item(QString input)
{
    int output = m_parser_data->new_break_item (input);
    setBreak_list (m_parser_data->break_list ());
    return output;
}

int part_object::delete_break_item(int measure_number)
{
    int output = m_parser_data->delete_break_item(measure_number);
    setBreak_list (m_parser_data->break_list ());
    return output;
}

int part_object::update_break_list(QString id, QString input)
{
    int output = m_parser_data->update_break_list (id, input);
    setBreak_list (m_parser_data->break_list ());
    return output;
}

void part_object::apply_breaks()
{
    m_parser_data->apply_breaks ();
    setBreak_list (m_parser_data->break_list ());
}

QVariantList part_object::element_from_point(const QPointF &point, const int &page_number)
{
    return m_xml_parser->element_from_point (point, page_number);
}



QString part_object::get_part_name()
{
    return m_part_name;
}





QStringList part_object::list_PNG_paths() const
{
    return m_list_PNG_paths;
}

void part_object::setlist_PNG_paths(const QStringList &newList_PNG_paths)
{
    if (m_list_PNG_paths == newList_PNG_paths)
        return;
    m_list_PNG_paths = newList_PNG_paths;
    emit list_PNG_pathsChanged();
}

parser_data *part_object::get_parser_data() const
{
    return m_parser_data;
}

QVariantList part_object::reh_y_coords() const
{
    return m_reh_y_coords;
}

void part_object::setReh_y_coords(const QVariantList &newReh_y_coords)
{
    if (m_reh_y_coords == newReh_y_coords)
        return;
    m_reh_y_coords = newReh_y_coords;
    emit reh_y_coordsChanged();
}

QVariantList part_object::break_list() const
{
    return m_break_list;
}

void part_object::setBreak_list(const QVariantList &newBreak_list)
{
    if (m_break_list == newBreak_list)
        return;
    m_break_list = newBreak_list;
    emit break_listChanged();
}

QString part_object::file_path() const
{
    return m_file_path;
}

void part_object::setfile_path(const QString &newFile_path)
{
    if (m_file_path == newFile_path)
        return;
    m_file_path = newFile_path;
    emit file_pathChanged();
}
