#include "render_file.h"
#include "globals.h"

render_file::render_file(QObject *parent,
                         class mei_parser *mei_parser,
                         class xml_parser *xml_parser,
                         class parser_data *parser_data,
                         class verovio_loader *verovio_loader,
                         class resvg_loader *resvg_loader)
    : QObject{parent}
{
    m_mei_parser = mei_parser;
    m_xml_parser = xml_parser;
    m_parser_data = parser_data;
    m_verovio_loader = verovio_loader;
    m_resvg_loader = resvg_loader;

    pagecount = 0;

    directory = QDir::currentPath() + "/UserFiles/";
    m_output_path = "";

    file_methods = new file_open();

    relative_path = "";
    image_path = "";

    refresher = 2;
}



void render_file::openFile(const QString &file_path, int mode)
{

    // make a project dump folder (userfiles/dump/project_name)
    qInfo() << "file path: " << file_path;

    if (mode == 0) {

        QString dir_path = directory + "dump/" + file_methods->name_from_project_files(file_path);
        QString track_path = dir_path + "/tracks";
        QString options_path = track_path + "/options";

        QDir directory(dir_path);
        QDir track_dir(track_path);
        QDir options_dir(options_path);

        if (!directory.exists()) {
            directory.mkpath (".");
        }

        if (!track_dir.exists()) {
            track_dir.mkpath(".");
        }

        if (!options_dir.exists()) {
            options_dir.mkpath(".");
        }
    }


    m_verovio_loader->load(file_path);

    relative_path = file_methods->name_from_project_files(file_path); // Get the relative path of the file
    setfilename(QFileInfo(relative_path).completeBaseName ());

    setfilename (QFileInfo(relative_path).completeBaseName ());
    // Set the filename property to the file name without path);

    render_data();

}

void render_file::update()
{
    m_verovio_loader->load_data(m_mei_parser->export_mei_data());
    render_data();
}




void render_file::render_data()
{
    refresher = 2/refresher; // Used to refresh the rendering process

    QStringList temp_list;

    m_xml_parser->clear_data();

    pagecount = m_verovio_loader->get_page_count (); // Get the page count from the Verovio loader;

    for (int current_page = 1; current_page <= pagecount; ++current_page) {
        // qInfo() << Qt::endl << Qt::endl << Qt::endl;
        // qInfo() << "Rendering page:" << current_page << " out of " << pagecount;
        // qInfo() << Qt::endl << Qt::endl << Qt::endl;



        QString image_path_temp = relative_path + "---pg_" + QString::number(current_page) + QString::number(refresher);

        //qInfo() << "checkpoint";

        std::string svg_data = m_verovio_loader->render(current_page);

        //qInfo() << "checkpoint";

        svg_data = m_xml_parser->parse_xml(QString::fromStdString(svg_data));// Parse the XML for the current page

        //qInfo() << "checkpoint";
        //qInfo() << "image://image_provider/"  + image_path_temp;

        m_resvg_loader->add_to_image_provider(svg_data, image_path_temp); // Add the SVG to the image provider

        temp_list << ("image://image_provider/"  + image_path_temp);    // Add the image path to the list and set it to the property so QML sees
        setlist_PNG_paths (temp_list);

        //qInfo() << "image://image_provider/"  + image_path_temp;

        // setoutput_path(image_path); // Set the output path for the rendered images (/rendered_PNGs/project_name)

        // QImage blank_image(2100, 5000, QImage::Format_ARGB32);
        // blank_image.fill(Qt::white);

        //g_image_provider->addImage("reloader", blank_image);

    }

    m_mei_parser->parse_mei(QString::fromStdString(m_verovio_loader->get_mei_data())); // get MEI elements
    m_parser_data->parse();



}




QString render_file::filename() const
{
    return m_filename;
}

void render_file::setfilename(const QString &newFilename)
{
    if (m_filename == newFilename)
        return;
    m_filename = newFilename;
    emit filenameChanged();
}


QString render_file::output_path() const
{
    return m_output_path;
}

void render_file::setoutput_path(const QString &newOutput_path)
{
    if (m_output_path == newOutput_path)
        return;
    m_output_path = newOutput_path;
    emit output_pathChanged();
}

QString render_file::make_directory(const QString &file_path)
{
    QDir directory(file_path);
    if (!directory.exists()) {
        directory.mkpath(".");
        qInfo() << "Directory created:" << file_path; // Log to console that the directory has been created
        return file_path;   // if the directory does not exist, create it and return the path
    }
    else {                  // if the directory already exists, make a new directory with a number added on
        int counter = 0;
        while (directory.exists()) {
            counter++;
            directory.setPath(file_path + "_" + QString::number(counter));
        }
        directory.mkpath(".");
        setfilename(QFileInfo(file_path).completeBaseName() + "_" + QString::number(counter));
        qInfo() << "Directory already exists, creating new directory with counter:" << counter; // Log to console that the directory already exists and a new one is being created
        return QString(file_path + "_" + QString::number(counter));
    }
}

QStringList render_file::list_PNG_paths() const
{
    return m_list_PNG_paths;
}

void render_file::setlist_PNG_paths(const QStringList &newList_PNG_paths)
{
    if (m_list_PNG_paths == newList_PNG_paths)
        return;
    m_list_PNG_paths = newList_PNG_paths;
    emit list_PNG_pathsChanged();
}


