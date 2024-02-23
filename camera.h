#ifndef CAMERA_H
#define CAMERA_H

#include <QWidget>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QCameraInfo>
#include <QDateTime>

namespace Ui {
class Camera;
}

class Camera : public QWidget
{
    Q_OBJECT

public:
    explicit Camera(QWidget *parent = nullptr);
    ~Camera();
    QCamera * camera;
    QCameraViewfinder * cameraViewfinder;
    QList<QCameraInfo> CameraInfolist;
    QCameraImageCapture * photo;

private:
    Ui::Camera *ui;
};

#endif // CAMERA_H
