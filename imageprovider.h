#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QObject>
#include <QMutex>
#include <opencv2/opencv.hpp>

class ImageProvider : public QObject, public QQuickImageProvider
{
    Q_OBJECT
public:
    explicit ImageProvider(QObject *parent = nullptr);
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

signals:
    void imageChanged();
    void sigProcessImg(QUrl);

public slots:
    void processImage(QVariant imgVar, QUrl path);
    void slot_imgFinished(QImage);

private:
    QMutex* _dataLock;
    QImage  _image;
    cv::Mat _matImg;
};

#endif // IMAGEPROVIDER_H
