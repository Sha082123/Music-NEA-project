#ifndef SCAN_EXISTING_H
#define SCAN_EXISTING_H

#define VEROVIO_DIR "@VEROVIO_DIR@"

/* scan existing directory for existing files
   create preview images of these files by using verovio to load the first page of the score
   previews saved to UserFiles/previews     */

#include <QObject>
#include <QFile>
#include <QDirIterator>
#include <QDir>
#include <QStringList>


class scan_existing : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList list_existing_files READ list_existing_files WRITE setlist_existing_files NOTIFY list_existing_filesChanged FINAL)

public:

    explicit scan_existing(QObject *parent = nullptr);
    Q_INVOKABLE void scan_directory(const QString &directory);
    Q_INVOKABLE QString get_previous_directory(const QString &current_directory); //returns the previous directory
    //of the current directory, unless
    //current directory is project_files then it returns project_files directory

    QStringList list_existing_files() const;
    void setlist_existing_files(const QStringList &newList_existing_files);

signals:

    void list_existing_filesChanged();

private:
    QString existing_directory;
    QStringList m_list_existing_files;
    void create_preview_images(const QStringList &file_list);
    QString base_directory;
    QString preview_directory;

};

#endif // SCAN_EXISTING_H
