#ifndef RENDER_FILE_H
#define RENDER_FILE_H

#define VEROVIO_DIR "@VEROVIO_DIR@"

#include <QObject>
#include <QFile>
#include <QDataStream>
#include <QTextStream>
#include <QDebug>
#include <QFileInfo>
#include <QString>
#include <QDir>


#include "../file_opening/file_open.h"

class render_file : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString filename READ filename WRITE setfilename NOTIFY filenameChanged FINAL)
    Q_PROPERTY(QString output_path READ output_path WRITE setoutput_path NOTIFY output_pathChanged FINAL)
    Q_PROPERTY(QStringList list_PNG_paths READ list_PNG_paths WRITE setlist_PNG_paths NOTIFY list_PNG_pathsChanged FINAL)

public:
    explicit render_file(QObject *parent = nullptr);
    Q_INVOKABLE void openFile(const QString &file_path, QString mode); //mode: new for new files, exisiting for existing files
    Q_INVOKABLE void update();

    QString filename() const;
    void setfilename(const QString &newFilename);

    QString output_path() const;
    void setoutput_path(const QString &newOutput_path);

    QStringList list_PNG_paths() const;
    void setlist_PNG_paths(const QStringList &newList_PNG_paths);

    void render_data();

signals:

    void filenameChanged();

    void output_pathChanged();

    void list_PNG_pathsChanged();

private:

    QString make_directory(const QString &file_path);

    void write_to_existing_files(const QString &file_path);

    QString m_filename;
    int pagecount;

    QString directory;

    QString relative_path;
    QString image_path;

    QString m_output_path;

    file_open* file_methods;
    QStringList m_list_PNG_paths;

    int refresher; // Used to refresh the rendering process
};

#endif // RENDER_FILE_H
