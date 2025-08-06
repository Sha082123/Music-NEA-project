#include "file_open.h"

file_open::file_open(QWidget *parent, verovio_loader *verovio_loader)
    : QWidget{parent}
{
    m_verovio_loader = verovio_loader;
}

QString file_open::openNewFile(const QString &target_dir) //target_dir is the directory where the file should be copied to !!ABSOLUTE PATH
{
    QString file_path = QFileDialog::getOpenFileName(this, "Open file", "C://"); //Replace later with directory of latest file
    //log to console that a file has been opened
    qInfo() << "File opened:" << file_path << Qt::endl;

    file_path = copy_file_to_projects(file_path, target_dir);

    qInfo() << "File copied to project files:" << file_path << Qt::endl;

    return file_path;
}

QString file_open::createNewPart(const QString &source_path, const QString &target_dir, const QString &file_name)
{
    if (file_name.isEmpty()) {
        qInfo() << "File name is empty, using base name from source path.";
        return copy_file_to_projects(source_path, target_dir);
    }

    return copy_file_to_projects(source_path, target_dir, file_name);
}

void file_open::openNewDirectory(const QString &target_dir)
{
    // QString correct_dir; // target_dir is constant, so we cannot remove the trailing slash directly from it.
    // correct_dir = target_dir;
    // if (correct_dir.endsWith("/")) {
    //     qInfo() << "Target directory ends with /, removing it.";
    //     correct_dir.chop(1); // Remove the trailing slash if it exists
    // }
    // QDir directory(correct_dir);
    // if (!directory.exists()) {
    //     directory.mkpath(".");
    //     qInfo() << "Directory created:" << correct_dir + "/"; // Log to console that the directory has been created

    // }
    // else {                  // if the directory already exists, make a new directory with a number added on
    //     int counter = 0;
    //     while (directory.exists()) {
    //         counter++;
    //         directory.setPath(correct_dir + "_" + QString::number(counter));
    //     }
    //     directory.mkpath(".");
    //     qInfo() << "Directory already exists, creating new directory with counter:" << counter; // Log to console that the directory already exists and a new one is being created
    // }

    QString dir_path = if_dir_exists (target_dir);
    QDir directory(dir_path);
    directory.mkpath(".");

}

QString file_open::name_from_project_files(const QString &file_path) //if file_path = D:/(PROJECT PATH)/UserFiles/project_files/MyProject/file.musicxml,
                                                                     //I want it to return MyProject/file
                                                                     //if directory is fed, then i want it to return the latest directory
{
    QFileInfo info(file_path); //check if file_path is a directory or string
    if (info.isDir()) {
        QDir dir(file_path);
        //qInfo() << file_path << " is a directory";
        //qInfo() << "returned : " << dir.dirName();
        return dir.dirName() + "/"; //if file_path is a directory, return the directory name
    }

    else {

        QDir base_dir(QDir::currentPath() + "/UserFiles/project_files/");
        QString relative_path = base_dir.relativeFilePath(file_path);    //returns MyProject/file.musicxml
        QFileInfo file_info(relative_path);
        QString result;
        QString relative_directory = file_info.path(); //returns MyProject or "." if file is not in folder

        if (relative_directory == "." || relative_directory.isEmpty()) {
            result = file_info.completeBaseName (); // Just "file"
        } else {
            result = relative_directory + "/" + file_info.completeBaseName (); // "directory/file"
        }

        return result; //return file
    }
}

QString file_open::get_base_name(const QString &file_path)
{
    QString correct_path = file_path;
    if (correct_path.endsWith("/")) {
        correct_path.chop(1);       // Remove the trailing slash if it exists
    }

    QFileInfo info(correct_path);
    return info.completeBaseName ();
}

QString file_open::get_current_dir()
{
    return QDir::currentPath();
}

bool file_open::isDir(QString path)
{
    return QFileInfo(path).isDir(); // Check if the path is a directory
}

void file_open::delete_file_or_dir(const QString &file_path)
{
    QFileInfo info(file_path);
    if (info.isDir()) {
        QDir dir(file_path);
        if (dir.removeRecursively()) {
            qInfo() << "Directory deleted:" << file_path;
        }
    } else {
        if (QFile::remove(file_path)) {
            qInfo() << "File deleted:" << file_path;
        }
    }
}

void file_open::rename_file_or_dir(const QString &old_path, const QString &new_name)
{
    QFileInfo info(old_path);

    if (info.isDir()) {
        QDir dir(old_path);
        dir.cdUp();
        QString new_path = dir.path() + "/" + new_name; // Construct new directory path

        qInfo() << "new path : " << new_path;
        qInfo() << "old path : " << old_path;
        if (!(new_path + "/" == old_path)) {                  // do nothing if both paths are the same
            new_path = if_dir_exists(new_path);        // Check if the directory already exists and adjust the name if necessary
            dir.rename(get_base_name(old_path), get_base_name(new_path)); //already has trailing slash
            qInfo() << "Directory renamed from" << old_path << "to" << new_path;
        }
    }
    else {
        QString new_path = info.path() + "/" + new_name + "." + info.suffix();
        if (new_path != old_path) {                  // do nothing if both paths are the same
            new_path = if_file_exists (new_path);       // Construct new file path with the same extension;
            QFile::rename(old_path, new_path);
            qInfo() << "File renamed from" << old_path << "to" << new_path;
        }
    }
}


QString file_open::copy_file_to_projects(const QString &source_path, const QString &target_dir, const QString &file_name)
{
    qInfo() << Qt::endl << Qt::endl << Qt::endl << "target_dir : " << target_dir;
    qInfo() << "source_path : " << source_path;

    QFileInfo file(source_path);

    QString temp_file_name;

    if (file_name.isEmpty()) {
        temp_file_name = file.completeBaseName();
    } else {
        temp_file_name = file_name;
    }


    QString file_path = target_dir + temp_file_name + ".mei";

    file_path = if_file_exists (file_path); //obtain the correct file_path

    //qInfo() << "destination path : " << file_path;

    m_verovio_loader->load (source_path);

    //qInfo() << "source path loaded: " + source_path;

    QString mei_data = QString::fromStdString (m_verovio_loader->get_mei_data());

    QFile writer(file_path);
    if (writer.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&writer);
        out << mei_data;
        writer.close();
    }

    return file_path;
}

QString file_open::if_file_exists(const QString file_path) //checks with project files
{
    QFileInfo info(file_path);
    QString baseName = info.completeBaseName ();
    QString extension = info.suffix ();
    QString file_dir = info.path();
    QStringList existing_files = QDir(file_dir).entryList(QStringList() << baseName + ".*", QDir::Files);

    qInfo() << "Base name:" << baseName;
    qInfo() << "Extension:" << extension;
    qInfo() << "File directory:" << file_dir;
    qInfo() << "File path var : " << file_path;
    qInfo() << "Existing files matching pattern:" << existing_files;

    if (existing_files.isEmpty()) {
        qInfo() << "file did not exist so new file created";
        return file_path;
    }
    else {
        int counter = 0;
        while (!existing_files.isEmpty()) {
            counter ++;
            QString next_test = baseName + "_" + QString::number(counter) + ".*";
            existing_files = QDir(file_dir).entryList(QStringList() << next_test, QDir::Files);
            qInfo() << "next_text : " << next_test;
            qInfo() << "existing files : " << existing_files;
        }
        qInfo() << "File with name " << baseName << " already exists, creating new file with counter: " << counter << Qt::endl;
        qInfo() << "returned : " << file_dir + "/" + baseName + "_" + QString::number(counter) + "." + extension;
        return file_dir + "/" + baseName + "_" + QString::number(counter) + "." + extension;
    }
}

QString file_open::if_dir_exists(const QString dir_path)
{
    QString correct_dir; // target_dir is constant, so we cannot remove the trailing slash directly from it.
    correct_dir = dir_path;
    if (correct_dir.endsWith("/")) {
        qInfo() << "Target directory ends with /, removing it.";
        correct_dir.chop(1); // Remove the trailing slash if it exists
    }
    QDir directory(correct_dir);
    if (!directory.exists()) {
        qInfo() << "Directory created:" << correct_dir + "/"; // Log to console that the directory has been created
        return correct_dir + "/"; // Create the directory and return the path with trailing slash;

    }
    else {                  // if the directory already exists, make a new directory with a number added on
        int counter = 0;
        while (directory.exists()) {
            counter++;
            directory.setPath(correct_dir + "_" + QString::number(counter));
        }
        qInfo() << "Directory already exists, creating new directory with counter:" << counter; // Log to console that the directory already exists and a new one is being created
        return directory.path() + "/";
    }

}
