#ifndef IMAGE_PROVIDER_H
#define IMAGE_PROVIDER_H

#include <QQuickImageProvider>
#include <QMap>
#include <QDebug>

class image_provider : public QQuickImageProvider
{
    Q_OBJECT
public:
    image_provider();

    void addImage(const QString& id, const QImage& img);

    QImage requestImage(const QString& id, QSize *size, const QSize &requestedSize) override;

private:
    QMap<QString, QImage> imageMap;
};

#endif // IMAGE_PROVIDER_H
