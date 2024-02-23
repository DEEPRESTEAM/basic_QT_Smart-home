#ifndef MONITORING_H
#define MONITORING_H

#include <QWidget>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QCameraInfo>
#include <QDateTime>

namespace Ui {
class Monitoring;
}

class Monitoring : public QWidget
{
    Q_OBJECT

public:
    explicit Monitoring(QWidget *parent = nullptr);
    ~Monitoring();
    QCamera * camera;
    QCameraViewfinder * cameraViewfinder;
    QList<QCameraInfo> CameraInfolist;
    QCameraImageCapture * photo;

private:
    Ui::Monitoring *ui;
};

#endif // MONITORING_H
