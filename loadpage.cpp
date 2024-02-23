#include "loadpage.h"
#include "ui_loadpage.h"
#include <QTimer>
#include <QDebug>

LoadPage::LoadPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoadPage)
{
    this->value = 0;
    ui->setupUi(this);
    this->setWindowTitle("Load");
    QTimer * time = new QTimer;

    ui->progressBar->setValue(0);
    ui->progressBar->setRange(0 , 2);
    ui->progressBar->setMaximum(2);
    connect(this , &LoadPage::start , this , [=](){
         time->start(1000);
    });
    connect(time , &QTimer::timeout , this , [=](){
        value++;
        ui->progressBar->setValue(value);
    });
    connect(ui->pushButton , &QPushButton::clicked , this ,[=](){
         value = 0;
         time->stop();
         ui->progressBar->reset();
         ui->progressBar->setValue(0);
         emit disable();
    });
    connect(ui->progressBar , &QProgressBar::valueChanged , this , [=](){
        if(ui->progressBar->value() == 2){
            emit complete();
        }
    });
}

LoadPage::~LoadPage()
{
    qDebug()<<"加载页面被析构了";
    delete ui;
}
