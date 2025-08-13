#include "scan_existing.h"
#include "toolkit.h"
#include "lunasvg.h"
#include "globals.h"

using namespace lunasvg;

scan_existing::scan_existing(QObject *parent)
    : QObject{parent}
{

    base_directory = QDir::currentPath() + "/UserFiles/";
    existing_directory = base_directory + "project_files/";
    preview_directory = base_directory + "previews/";

}

void scan_existing::scan_directory(const QString &directory)
{
    QStringList temp_list = {};

    // if (directory != existing_directory) {
    //     temp_list << get_previous_directory(directory); //add previous directory to the list
    //     qInfo() << "current directory is : " << directory;
    //     qInfo() << "previous directory is: " << temp_list.last();
    // }

    QDirIterator file(directory, QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot); //add immediate files and immediate directories
    while (file.hasNext()) {
        QString next_file = file.next();
        if (QFileInfo(next_file).isDir()) {
            temp_list << (next_file + "/"); //add trailing slash if directory
        } else {
            temp_list << next_file; //add file to the list
        }
    }

    create_preview_images(temp_list);
    setlist_existing_files (temp_list);
}

QStringList scan_existing::list_existing_files() const
{
    return m_list_existing_files;
}

void scan_existing::setlist_existing_files(const QStringList &newList_existing_files)
{
    if (m_list_existing_files == newList_existing_files)
        return;
    m_list_existing_files = newList_existing_files;
    emit list_existing_filesChanged();
}

void scan_existing::create_preview_images(const QStringList &file_list)
{
    vrv::Toolkit tk;

    QDir dir(QDir::currentPath());
    dir.cdUp();
    dir.cdUp();
    QString verovio_resource_path = dir.path() + "/build/Desktop_Qt_6_9_1_MSVC2022_64bit-Release/external/verovio/data";
    qInfo() << "Verovio resource path: " << verovio_resource_path;

    tk.SetResourcePath (verovio_resource_path.toStdString());

    tk.SetOptions(R"({
        "scale": 50,
        "pageHeight": 2000,
        "pageWidth": 1000,
        "adjustPageHeight": true,
        "adjustPageWidth": true,
        "breaks": "smart",
        "condense": "none"
    })");

    for (const QString &file_path : file_list) { //file_path is to project_files
        QFileInfo info(file_path);
        if (!info.isDir()) { //if file_path is a file, not a directory, then proceed. Else, skip to next element.
                             //this is troublesome because imagine if the file_list contains files inside directories eg project_files/MyFolder/file.musicxml
                             //then in the previews file, only file.png will be created. What if there are duplicates?
                             //thats why i need to extract the MyFolder/file.musicxml part
            QFileInfo temp(file_path);
            QDir existing_dir(existing_directory);
            QString relative_path = existing_dir.relativeFilePath (temp.filePath ()); //remove base directory from path
            QFileInfo file_info(relative_path);

            QString relative_name; //im trying to create folder/file from folder/file.musicxml
            QString relative_base_name = file_info.completeBaseName(); //should be "file"
            QString relative_directory = file_info.path(); //should be "folder" or "." if relative name is just "file.musicxml"

            if (relative_directory == "." || relative_directory.isEmpty()) {
                relative_name = relative_base_name; // Just "file"
            } else {
                relative_name = relative_directory + "/" + relative_base_name; // "directory/file"
                QDir create_dir;
                create_dir.mkpath (preview_directory + relative_directory); //create PATH/previews/MyFolder
            }

            qInfo() << "file_path : " << file_path;
            qInfo() << "relative directory : " << relative_directory;
            qInfo() << "relative path : " << relative_path;
            qInfo() << "relative base name : " << relative_base_name;
            qInfo() << "relative directory : " << relative_directory;
            qInfo() << "relative name : " << relative_name;

            qInfo() << "preview_directory: " << preview_directory;

            QDir preview_dir(preview_directory);
            if (!preview_dir.exists(relative_name + ".png")) {
                //qInfo() << "checkpoint";
                tk.LoadFile (file_path.toStdString ()); //file path is the original file value passed into here, which should be UserFiles/project_files
                //qInfo() << "checkpoint";
                std::string svg_string = tk.RenderToSVG (1);

                //qInfo() << "checkpoint";
                auto buffer = Document::loadFromData(svg_string.data());

                //qInfo() << "checkpoint";
                auto bitmap = buffer->renderToBitmap();

                //qInfo() << "checkpoint";
                bitmap.writeToPng ((preview_directory + relative_name + ".png").toStdString ());

                //qInfo() << "checkpoint";
                qInfo() << "Preview saved to : " << preview_directory + relative_name + ".png";
            }
        }
    }
}

QString scan_existing::get_previous_directory(const QString &current_directory)
{
    qInfo() << "current_directory : " << current_directory;
    qInfo() << "existing_directory : " << existing_directory;
    if (current_directory != existing_directory){
        QDir dir(current_directory);
        dir.cdUp(); // Moves up one level
        QString parentDir = dir.absolutePath();
        qInfo() << "parent dir" << parentDir + "/";
        return parentDir + "/"; //returns one level higher of the current directory
    }
    else {
        return current_directory;
    }
}
