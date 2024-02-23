#include "monitoring.h"
#include "ui_monitoring.h"

Monitoring::Monitoring(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Monitoring)
{
    ui->setupUi(this);
    this->setWindowTitle("监控");
    //获取当前可用设备信息并放到选择栏里
    CameraInfolist = QCameraInfo::availableCameras();
    //设置开始界面显示
    this->ui->off->setPixmap(QPixmap(":/new/prefix2/picture/断开连接.jpg"));
    this->ui->off->show();
    this->ui->off->setFixedSize(800,400);
    this->ui->creamlabe->hide();
    this->ui->btn_take->setEnabled(false);
    this->ui->btn_close->setEnabled(false);

    for(QList<QCameraInfo>::iterator it = CameraInfolist.begin();it != CameraInfolist.end();it++)
    {
        this->ui->cream_list->addItem(it->description());
    }

    //设置并绑定开始按钮
    connect(this->ui->btn_start,&QPushButton::clicked,this,[=](){
        this->ui->off->hide();
        this->ui->creamlabe->show();
        //初始化摄像头基本属性
        QCameraInfo Camerainfo = CameraInfolist.at(ui->cream_list->currentIndex());
        camera = new QCamera(Camerainfo);
        this->cameraViewfinder = new QCameraViewfinder(this->ui->creamlabe);
        this->camera->setViewfinder(cameraViewfinder);

        this->cameraViewfinder->resize(800,400);
        cameraViewfinder->show();
        this->camera->start();

        //设置按钮
        this->ui->btn_take->setEnabled(true);
        this->ui->btn_close->setEnabled(true);
        this->ui->btn_start->setEnabled(false);

        //设置拍照
        this->photo = new QCameraImageCapture(camera);
        connect(this->ui->btn_take,&QPushButton::clicked,this,[=](){
            emit this->photo->capture();
        });
        connect(this->photo,&QCameraImageCapture::imageCaptured,this,[=](int id, const QImage &preview){
            QDateTime nowtime(QDateTime::currentDateTime());
            QString time = nowtime.toString("yyyy-MM-dd-hh-mm-ss");

            QString filename = QString("../%1-%2.jpg").arg(time).arg(id);
            preview.save(filename);
            qDebug()<<"拍摄成功";
        });
    });

    //设置关闭按钮
    connect(this->ui->btn_close,&QPushButton::clicked,this,[=](){
        this->camera->stop();
        //隐藏摄像头窗口
        this->ui->off->show();
        this->ui->off->setFixedSize(800,400);
        this->ui->creamlabe->hide();

        //设置按钮
        this->ui->btn_take->setEnabled(false);
        this->ui->btn_close->setEnabled(false);
        this->ui->btn_start->setEnabled(true);
    });
}

Monitoring::~Monitoring()
{
    qDebug()<<"监控系统被析构";
    delete ui;
}
