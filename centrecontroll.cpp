#include "centrecontroll.h"
#include "ui_centrecontroll.h"
#include "weatheritem.h"
#include <QDebug>
CentreControll::CentreControll(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CentreControll)
{
    ui->setupUi(this);
    this->setWindowTitle("中心控制器");
    //指针初始化
    musicItem = new MusicItem();
    musicItem->hide();
    weather = new weatherItem();
    weather->hide();

    smartHomeItem = new SmartHome();
    smartHomeItem->hide();

    monitoringItem = new Monitoring();
    monitoringItem->hide();
    //实现音乐按钮
    connect(ui->btnMusic , &QPushButton::clicked , this , [=](){
        this->hide();
        musicItem->show();
    });
    connect(musicItem , &MusicItem::musicOver , this , [=](){
        this->show();

        musicItem->hide();
    });

    //实现智能家居按钮
    connect(ui->btnSmart , &QPushButton::clicked , this , [=]{
        this->hide();
        smartHomeItem->show();
    });
    connect(smartHomeItem , &SmartHome::over , this , &CentreControll::show);

    //实现天气预报按钮
    connect(ui->btnWeather , &QPushButton::clicked , this , [=](){
        weather->show();
    });

    //实现监控按钮
    connect(ui->btn , &QPushButton::clicked , this , [=](){
        monitoringItem->show();
    });

    //退出系统按钮
    connect(ui->btnBack , &QPushButton::clicked , this , [=](){
        musicItem->close();
        smartHomeItem->close();
        weather->close();
        this->close();

    });
}

CentreControll::~CentreControll()
{
    qDebug()<<"中心界面被析构了";
    delete ui;
    delete musicItem;
    delete smartHomeItem;
    delete weather;
    delete monitoringItem;
}

void CentreControll::closeEvent(QCloseEvent *)
{
    musicItem->close();
    smartHomeItem->close();
    weather->close();
    monitoringItem->close();
    this->close();
}
