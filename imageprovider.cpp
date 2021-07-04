#include "imageprovider.h"

#include <QtConcurrent/QtConcurrent>
#include <QException>
#include <QThread>

ImageProvider::ImageProvider(QObject *parent) : QObject(parent), QQuickImageProvider(QQmlImageProviderBase::Image)
{
}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    try {
        Q_UNUSED(id);

        if(size){
            *size = _image.size();
        }

        if(requestedSize.width() > 0 && requestedSize.height() > 0) {
            _image = _image.scaled(requestedSize.width(), requestedSize.height(), Qt::KeepAspectRatio);
        }
        return _image;
    } catch (QException e) {
        qDebug() << "requestImage [ERROR]: " << e.what();
    }
}

void ImageProvider::processImage(QVariant imgVar, QUrl path)
{
    try {
        QImage img = qvariant_cast<QImage>(imgVar);
        if (img.width() > 0 && img.height() > 0) {
            // Enviando sinal para ProcessWorker (Thread Separada)
            emit sigProcessImg(path);
        }
        else {
            qDebug() << "[ERROR]: Img is NULL";
        }
    } catch (QException e) {
        qDebug() << "Provider [ERROR]: " << e.what();
    }

}

void ImageProvider::slot_imgFinished(QImage image)
{
    try {
        _image = image;
        emit imageChanged();
    } catch (QException e) {
        qDebug() << "Provider [ERROR]: " << e.what();
    }
}
