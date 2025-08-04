#ifndef FILE_OPEN_H
#define FILE_OPEN_H

#include <QObject>
#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QFileInfo>
#include <globals.h>

class file_open : public QWidget
{
    Q_OBJECT
public:
    explicit file_open(QWidget *parent = nullptr);
    Q_INVOKABLE QString openNewFile(const QString &target_dir); //file path should be target directory
    Q_INVOKABLE void openNewDirectory(const QString &target_dir); //target_dir is the directory where the file should be copied
    Q_INVOKABLE QString name_from_project_files(const QString &file_path); //get name including folder from project_files
    Q_INVOKABLE QString get_base_name(const QString &file_path); //get base name of the file without extension)
    Q_INVOKABLE QString get_current_dir();
    Q_INVOKABLE bool isDir(QString path);

    Q_INVOKABLE void delete_file_or_dir(const QString &file_path); //delete the file from project_files
    Q_INVOKABLE void rename_file_or_dir(const QString &file_path, const QString &new_name); //rename the file or directory in project_files
signals:


private:
    QString copy_file_to_projects(const QString &source_path, const QString &target_dir);
    QString if_file_exists(const QString file_path); //check if the file exists in project_files
                                                     //check if the filename exists in project_files
                                                     //returns the original file path if not, otherwise adds a number
    QString if_dir_exists(const QString dir_path); //check if the directory exists in project_files
                                           //check if the directory name exists in project_files
                                           //returns the original directory path if not, otherwise adds a number

};



#endif // FILE_OPEN_H
