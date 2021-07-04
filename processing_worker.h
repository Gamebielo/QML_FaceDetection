#ifndef PROCESSING_WORKER_H
#define PROCESSING_WORKER_H

#include <QObject>
#include <QString>
#include <QImage>
#include <QDebug>
#include <QFile>
#include <QUrl>

#include <opencv2/opencv.hpp>

class Processing_Worker : public QObject
{
    Q_OBJECT
public:
    explicit Processing_Worker(QObject *parent = nullptr);

signals:
    void sig_imgFinished(QImage);

public slots:
    void slot_threadStarted();
    void slot_openProcessImg(QUrl);

private:
    QImage _image;
};

#endif // PROCESSING_WORKER_H
