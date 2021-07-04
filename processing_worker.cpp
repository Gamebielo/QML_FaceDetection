#include "processing_worker.h"
#include <QStandardPaths>
#include <QException>
#include <QThread>

Processing_Worker::Processing_Worker(QObject *parent) : QObject(parent)
{

}

void Processing_Worker::slot_threadStarted()
{
    qDebug() << "Processing_Worker Thread: " << QThread::currentThreadId();

    QString xmlFile = "/haarcascadeFrontalfaceDefault.xml";
    // /data/user/0/projectPacket/files/...
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    // Copiando o arquivo xml do apk para uma pasta do dispositivo
    QFile assets_path("assets:/assets/" + xmlFile);
    // /data/user/0/projectPacket/files/haarcascadeFrontalfaceDefault.xml
    assets_path.copy(path + xmlFile);
    QFile::setPermissions(path + xmlFile, QFileDevice::ReadUser | QFileDevice::WriteUser);
}

void Processing_Worker::slot_openProcessImg(QUrl filePath)
{
    try {
        // Lendo a imagem novamente (QUrl path)
        cv::Mat tempFrame = cv::imread(filePath.toLocalFile().toStdString());
        cv::Mat grayFrame, faceFrame;
        cv::resize(
                    tempFrame,
                    tempFrame,
                    cv::Size(tempFrame.size().width / 3.0, tempFrame.size().height / 3.0)
                    );
        // transformando a imagem em escala de cinza
        cv::cvtColor(tempFrame, grayFrame, cv::COLOR_BGR2GRAY);

        // xmls e database são lidos em "/data/user/0/projectPacket/files/"

        cv::CascadeClassifier faceDecet = cv::CascadeClassifier();
        QString xmlFile = "/haarcascadeFrontalfaceDefault.xml";
        // /data/user/0/projectPacket/files/...
        QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        faceDecet.load(path.toStdString() + xmlFile.toStdString());
        if (faceDecet.empty()) {
            if (!faceDecet.load(path.toStdString() + xmlFile.toStdString()))
            {
                qDebug() << "[ERROR]: Error ao ler arquivo XML.";
            }
        }

        std::vector<cv::Rect> faces;
        faceDecet.detectMultiScale(
                    grayFrame,
                    faces, 1.1, 3, 0,
                    cv::Size(30, 30)
                    );

        for( int i = 0; i < faces.size(); i++)
            {
                cv::Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
                cv::ellipse(
                            tempFrame,
                            center,
                            cv::Size( faces[i].width*0.5, faces[i].height*0.5),
                            0, 0, 360,
                            cv::Scalar(0, 255, 0),
                            4, 8, 0
                            );
            }

        _image = QImage(tempFrame.clone().data,
                        tempFrame.clone().cols,
                        tempFrame.clone().rows,
                        tempFrame.clone().step,
                        QImage::Format_RGB888
                  ).rgbSwapped();

        emit sig_imgFinished(_image);
    } catch (QException e) {
        qDebug() << "ProcWorker [ERROR]: " << e.what();
    }
}
