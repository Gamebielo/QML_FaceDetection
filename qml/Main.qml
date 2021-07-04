import Felgo 3.0
import QtQuick 2.0
import QtQuick.Layouts 1.12

App {
    id: appRoot
    property url imgPath: ""
    onInitTheme: {
        Theme.navigationBar.backgroundColor = "#3F51B5"
    }

    NavigationStack {
        Page {
            id: mainPage
            // Title Options
            title: Theme.navigationBar.titleAlignLeft = false // Center Title
            titleItem:
                AppText {
                            text: "Detector de Faces"
                            color: "white"
                            fontSize: 20
                            font.bold: true
                        }
            // Imagem principal centralizada
            AppImage {
                id: image
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                width: parent.width - 50
                height: parent.height - 250
                asynchronous: true
                autoTransform: true
                fillMode: Image.PreserveAspectFit
                cache: false
            }
            // Linha dos botões
            Row {
                x: -38
                y: -67
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                anchors.horizontalCenter: parent.horizontalCenter

                AppButton {
                    id: btnCamera
                    text: "Camera"
                    radius: 10
                    onClicked: {
                        nativeUtils.displayCameraPicker("test")
                    }
               }

                AppButton {
                    id: btnLoad
                    text: "Load"
                    radius: 10
                    onClicked: nativeUtils.displayImagePicker("test")
                }

                AppButton {
                    id: btnProcess
                    text: "Process"
                    radius: 10
                    onClicked: {
                        if (image.sourceSize.width > 0)
                        {
                            image.grabToImage(function(result) {
                                imgProvider.processImage(result.image, imgPath);
                            });
                        }
                        else
                        {
                            console.log("No image loaded!")
                        }
                    }
                }
            }
            // Load Image (Gallery)
            Connections {
                target: nativeUtils
                onImagePickerFinished: {
                    if(accepted) {
                        imgPath = path
                        image.source = path
                    }
                }
            }
            // Reload Image UI (processed)
            Connections {
                target: imgProvider
                onImageChanged: image.source = "image://imgProvider/test"
            }
            // Native Camera Button
            Connections {
                target: nativeUtils
                onCameraPickerFinished: {
                    if(accepted) {
                        imgPath = path
                        image.source = path
                    }
                }
            }
        }
    }
}
