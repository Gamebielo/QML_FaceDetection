#include <QApplication>
#include <FelgoApplication>

#include <QQmlModuleRegistration>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>

#include "imageprovider.h"
#include "processing_worker.h"

// uncomment this line to add the Live Client Module and use live reloading with your custom C++ code
//#include <FelgoLiveClient>

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    FelgoApplication felgo;

    // Use platform-specific fonts instead of Felgo's default font
    felgo.setPreservePlatformFonts(true);

    QQmlApplicationEngine engine;
    QQmlContext* rootContext = engine.rootContext();
    felgo.initialize(&engine);

    // Set an optional license key from project file
    // This does not work if using Felgo Live, only for Felgo Cloud Builds and local builds
    felgo.setLicenseKey(PRODUCT_LICENSE_KEY);

    // use this during development
    // for PUBLISHING, use the entry point below
    felgo.setMainQmlFileName(QStringLiteral("qml/Main.qml"));

    QThread* processThread = new QThread();
    Processing_Worker* procWorker = new Processing_Worker();
    ImageProvider* imgProvider = new ImageProvider();

    rootContext->setContextProperty("imgProvider", imgProvider);
    rootContext->engine()->addImageProvider("imgProvider", imgProvider);

    engine.load(QUrl(felgo.mainQmlFileName()));
    //FelgoLiveClient liveClient(&engine);

    procWorker->moveToThread(processThread);
    QObject::connect(imgProvider, &ImageProvider::sigProcessImg, procWorker, &Processing_Worker::slot_openProcessImg);
    QObject::connect(processThread, &QThread::started, procWorker, &Processing_Worker::slot_threadStarted);
    QObject::connect(procWorker, &Processing_Worker::sig_imgFinished, imgProvider, &ImageProvider::slot_imgFinished);
    processThread->start();

    return app.exec();
}
