#ifndef SVGRENDERER_H
#define SVGRENDERER_H

#include <QObject>
#include <QSvgRenderer>
#include <QXmlStreamReader>
#include <QFile>
#include <QImage>
#include <QDebug>
#include <QSize>
#include <QPainter>
#include <QDir>

#include "image_provider.h"
#include "../globals.h"

class SvgRenderer : public QObject
{
    Q_OBJECT
public:
    explicit SvgRenderer(QObject *parent = nullptr);
    void Render_svg (const QString &file_path, const QString &output_path);
    void add_to_image_provider(const std::string &data, const QString &file_name); //file_name should be eg. iris--page_1.png

signals:

private:

};

#endif // SVGRENDERER_H
