#include "image_provider.h"
#include "globals.h"

image_provider::image_provider() : QQuickImageProvider(QQuickImageProvider::Image) {}

void image_provider::addImage(const QString &id, const QImage &img)
{
    imageMap[id] = img;

    //qInfo() << Qt::endl << Qt::endl << Qt::endl << "image id" << id;
    //qInfo() << "image data" << img << Qt::endl << Qt::endl << Qt::endl;
}

QImage image_provider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    //qInfo() << Qt::endl << Qt::endl << Qt::endl << "requestImage called for id:" << id <<Qt::endl << Qt::endl <<Qt::endl;
    QImage img = imageMap.value(id);

    //qDebug() << "requestImage for id:" << id << "found image:" << img << "isNull:" << img.isNull();

    if (size) *size = img.size();
    if ((!requestedSize.isEmpty()) && (img.size() != requestedSize))
        img = img.scaled(requestedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    //qInfo() << "Returning image with size:" << img.size() << Qt::endl << img << Qt::endl << Qt::endl;
    return img;
}
