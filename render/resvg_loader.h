#ifndef RESVG_LOADER_H
#define RESVG_LOADER_H

#include <QObject>
#include <QDir>
// #include "ResvgQt.h"
#include <QImage>
#include <lunasvg.h>

class resvg_loader : public QObject
{
    Q_OBJECT
public:
    explicit resvg_loader(QObject *parent = nullptr);
    QImage render(const std::string &svg_string);
    void add_to_image_provider(const std::string &data, const QString &file_name);
    void set_attribute(std::string element_id, const std::string& name, const std::string& value,
                       const QString &file_name, const std::string &file_data);

    void add_page_height(int &height);
    int get_page_height(int &page_number);

signals:

private:
    QString font_path;
    // ResvgRenderer* renderer; // Pointer to the Resvg instance
    // ResvgOptions* options;
    QString root;

    std::unique_ptr<lunasvg::Document> document;

    QVector<int> page_heights; // Store the heights of each page
};

#endif // RESVG_LOADER_H
