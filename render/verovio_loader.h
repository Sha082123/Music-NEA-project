#ifndef VEROVIO_LOADER_H
#define VEROVIO_LOADER_H

#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include "toolkit.h"

class verovio_loader : public QObject
{
    Q_OBJECT
public:
    explicit verovio_loader(QObject *parent = nullptr);
    void load(const QString &target_path); // Function to load Verovio resources
    void load_data(const QString &mei_data); // Function to load MEI data directly
    std::string render(const int &page_number); // Function to render a specific page
    int get_page_count();
    std::string get_mei_data();

signals:

private:
    QString verovio_resource_path; // Path to the Verovio resources
    QString options;
    vrv::Toolkit* tk; // Pointer to the Verovio toolkit instance;

};

#endif // VEROVIO_LOADER_H
