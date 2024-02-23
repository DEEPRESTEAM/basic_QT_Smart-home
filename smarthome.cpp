    #include "smarthome.h"
#include "ui_smarthome.h"

#include <QMessageBox>

SmartHome::SmartHome(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SmartHome)
{
    timer = new QTimer(this);
    ui->setupUi(this);
    this->setWindowTitle("智能家居");
    //先将所有样式设置好
    ui->statu->setText("未连接");
    ui->labelAir->setPixmap(QPixmap(":/smartHome/smartHome/空调-关.png"));
    ui->labelAir->setScaledContents(true);
    ui->labelAir_1->setScaledContents(true);
    ui->labelAir_1->setPixmap(QPixmap(":/smartHome/smartHome/空调-关.png"));
    ui->labelLamp->setPixmap(QPixmap(":/smartHome/smartHome/吊灯-关.png"));
    ui->labelLamp_1->setPixmap(QPixmap(":/smartHome/smartHome/吊灯-关.png"));
    ui->labelLamp_2->setPixmap(QPixmap(":/smartHome/smartHome/吊灯-关.png"));
    ui->labelLamp_3->setPixmap(QPixmap(":/smartHome/smartHome/吊灯-关.png"));
    ui->labelLamp_4->setPixmap(QPixmap(":/smartHome/smartHome/吊灯-关.png"));
    ui->labelLamp->setScaledContents(true);
    ui->labelLamp_1->setScaledContents(true);
    ui->labelLamp_2->setScaledContents(true);
    ui->labelLamp_3->setScaledContents(true);
    ui->labelLamp_4->setScaledContents(true);
    ui->labelFan->setPixmap(QPixmap(":/smartHome/smartHome/风扇-关.png"));
    ui->labelFan->setScaledContents(true);
    ui->lineTem->setReadOnly(true);

    //首先设置通讯，这个作为服务端
    //初始化TCpService
    this->service = new QTcpServer(this);
    this->socket = nullptr;

    //设置端口号
    ui->linePort->setText("20000");
    //当点击启动服务器时触发事件
    connect(ui->btnStart , &QPushButton::clicked , this , [=](){
        //启动服务器后端口号不能修改
        ui->linePort->setReadOnly(true);
        //然后获取里面的端口号值
        unsigned int portNum = ui->linePort->text().toUInt();
        //然后开始监听事件
        service->listen(QHostAddress::Any , portNum);
        QMessageBox::information(this , "启动服务器" , "启动成功");
        //将启动服务器按钮置为不可选中
        ui->btnStart->setEnabled(false);
        //当点击连接服务器之后按钮都为不可按下
        ui->btnli->setEnabled(false);
    });
    //如果有新的连接连接到服务器将出发此事件
    connect(service , &QTcpServer::newConnection , this , [=](){
        //当有新的连接可用时,获取通信的套接字
        socket = service->nextPendingConnection();
        ui->statu->setText("已连接");
        //服务器接受消息
        connect(socket , &QTcpSocket::readyRead , this , [=](){
            //首先服务器接受客户端发送来的消息
            QString date = socket->readAll();
            //qDebug()<<date;
            //这里处理客户端发送来的消息
            QStringList list = date.split(':' ,QString::SkipEmptyParts);
            if(list[0].toInt() == 1){
                //这里是处理所有房间的电器
                if(list[1] == "客厅"){
                    if(list[2] == "灯" && list[3].toInt() == 1){
                        //将灯打开
                        ui->labelLamp->setPixmap(QPixmap(":/smartHome/smartHome/吊灯-开.png"));
                        ui->btnLamp->setText("吊灯关");
                    }else if(list[2] == "灯" && list[3].toInt() == 0){
                        //将灯关闭
                        ui->labelLamp->setPixmap(QPixmap(":/smartHome/smartHome/吊灯-关.png"));
                        ui->btnLamp->setText("吊灯开");
                    }
                    if(list[2] == "空调" && list[3].toInt() == 1 ){
                        //首先需要将图片改了
                        ui->labelAir->setPixmap(QPixmap(":/smartHome/smartHome/空调-开.png"));
                        ui->btnAir->setText("空调关");
                    }else if(list[2] == "空调" && list[3].toInt() == 0){
                        ui->labelAir->setPixmap(QPixmap(":/smartHome/smartHome/空调-关.png"));
                        ui->btnAir->setText("空调开");
                    }
                }
                if (list[1] == "主卧") {
                    if(list[2] == "灯" && list[3].toInt() == 1){
                        //将灯打开
                        ui->labelLamp_1->setPixmap(QPixmap(":/smartHome/smartHome/吊灯-开.png"));
                        ui->btnLamp_1->setText("吊灯关");
                    }else if(list[2] == "灯" && list[3].toInt() == 0){
                        //将灯关闭
                        ui->labelLamp_1->setPixmap(QPixmap(":/smartHome/smartHome/吊灯-关.png"));
                        ui->btnLamp_1->setText("吊灯开");
                    }
                    if(list[2] == "空调" && list[3].toInt() == 1 ){
                        //将空调打开
                        ui->labelAir_1->setPixmap(QPixmap(":/smartHome/smartHome/空调-开.png"));
                        ui->btnAir_1->setText("空调关");
                    }else if(list[2] == "空调" && list[3].toInt() == 0){
                        //将空调关闭
                        ui->labelAir_1->setPixmap(QPixmap(":/smartHome/smartHome/空调-关.png"));
                        ui->btnAir_1->setText("空调开");
                    }

                }
                if (list[1] == "餐厅"){
                    if(list[2] == "灯" && list[3].toInt() == 1){
                        //将灯打开
                        ui->labelLamp_2->setPixmap(QPixmap(":/smartHome/smartHome/吊灯-开.png"));
                        ui->btnLamp_2->setText("吊灯关");
                    }else if(list[2] == "灯" && list[3].toInt() == 0){
                        //将灯关闭
                        ui->labelLamp_2->setPixmap(QPixmap(":/smartHome/smartHome/吊灯-关.png"));
                        ui->btnLamp_2->setText("吊灯开");
                    }
                    if (list[2] == "风扇" && list[3].toInt() == 1) {
                        ui->labelFan->setPixmap(QPixmap(":/smartHome/smartHome/风扇-开.png"));
                        ui->btnFan->setText("风扇关");
                    }else if(list[2] == "风扇" && list[3].toInt() == 0){
                        ui->labelFan->setPixmap(QPixmap(":/smartHome/smartHome/风扇-关.png"));
                        ui->btnFan->setText("风扇开");
                    }

                }
                if (list[1] == "厨房") {
                    if(list[2] == "灯" && list[3].toInt() == 1){
                        //将灯打开
                        ui->labelLamp_3->setPixmap(QPixmap(":/smartHome/smartHome/吊灯-开.png"));
                        ui->btnLamp_3->setText("吊灯关");
                    }else if(list[2] == "灯" && list[3].toInt() == 0){
                        //将灯关闭
                        ui->labelLamp_3->setPixmap(QPixmap(":/smartHome/smartHome/吊灯-关.png"));
                        ui->btnLamp_3->setText("吊灯开");
                    }

                }
                if (list[1] == "卫生间") {
                    if(list[2] == "灯" && list[3].toInt() == 1){
                        //将灯打开
                        ui->labelLamp_4->setPixmap(QPixmap(":/smartHome/smartHome/吊灯-开.png"));
                        ui->btnLamp_4->setText("吊灯关");
                    }else if(list[2] == "灯" && list[3].toInt() == 0){
                        //将灯关闭
                        ui->labelLamp_4->setPixmap(QPixmap(":/smartHome/smartHome/吊灯-关.png"));
                        ui->btnLamp_4->setText("吊灯开");
                    }

                }

            }else if(list[0].toInt() == 0){

                emit add();
                num = list[1].toInt();
                socket->write("0:26");
                ui->labelAir_1->setPixmap(QPixmap(":/smartHome/smartHome/空调-开.png"));
                ui->btnAir_1->setText("空调关");
                ui->labelAir->setPixmap(QPixmap(":/smartHome/smartHome/空调-开.png"));
                ui->btnAir->setText("空调关");

            }

        });
        connect(this , &SmartHome::add , this , [=](){
               timer->start(1000);
        });
        connect(timer , &QTimer::timeout , this , [=](){
                if(num > 26){
                    if((ui->btnAir->text() == "空调关" && ui->btnAir_1->text() == "空调开") || (ui->btnAir->text() == "空调开" && ui->btnAir_1->text() == "空调关")){
                        timer->start(2000);
                    }else if(ui->btnAir->text() == "空调开" && ui->btnAir_1->text() == "空调开"){
                        timer->stop();
                    }else {
                        timer->start(1000);
                    }
                    num--;
                    ui->lineTem->setText(QString().number(num));
                }else if(num < 26 ) {
                    if((ui->btnAir->text() == "空调关" && ui->btnAir_1->text() == "空调开") || (ui->btnAir->text() == "空调开" && ui->btnAir_1->text() == "空调关")){
                        timer->start(2000);
                    }else if(ui->btnAir->text() == "空调开" && ui->btnAir_1->text() == "空调开"){
                        timer->stop();
                    }else {
                        timer->start(1000);
                    }
                    num++;
                    ui->lineTem->setText(QString().number(num));
                }else {
                    timer->stop();
                    ui->labelAir_1->setPixmap(QPixmap(":/smartHome/smartHome/空调-关.png"));
                    ui->btnAir_1->setText("空调开");
                    ui->labelAir->setPixmap(QPixmap(":/smartHome/smartHome/空调-关.png"));
                    ui->btnAir->setText("空调开");
                }
        });
        //客户端断开连接
        connect(socket , &QTcpSocket::disconnected , this , [=](){
            //然后将状态栏改变
            ui->statu->setText("未连接");
            //然后将按钮置为可使用
            ui->btnli->setEnabled(true);
            //ui->btnStart->setEnabled(true);
            //然后断开连接
            socket->close();
            socket = nullptr;
        });

    });
    connect(ui->btnli , &QPushButton::clicked , this , &SmartHome::close);

    //接下来就是绑定发送按钮了
    //客厅按钮
    connect(ui->btnAir , &QPushButton::clicked , this , [=](){
        if(ui->btnAir->text() == "空调开"){
            //首先需要将图片改了
            ui->labelAir->setPixmap(QPixmap(":/smartHome/smartHome/空调-开.png"));
            if(socket != nullptr){
                socket->write("1:客厅:空调:1");
            }
            ui->btnAir->setText("空调关");
        }else if (ui->btnAir->text() == "空调关") {
            ui->labelAir->setPixmap(QPixmap(":/smartHome/smartHome/空调-关.png"));
            if(socket != nullptr){
                socket->write("1:客厅:空调:0");
            }
            ui->btnAir->setText("空调开");
        }
    });
    connect(ui->btnLamp , &QPushButton::clicked , this , [=](){
        if(ui->btnLamp->text() == "吊灯开"){
            ui->labelLamp->setPixmap(QPixmap(":/smartHome/smartHome/吊灯-开.png"));
            if(socket != nullptr){
                socket->write("1:客厅:吊灯:1");
            }
            ui->btnLamp->setText("吊灯关");
        }else if (ui->btnLamp->text() == "吊灯关") {
            ui->labelLamp->setPixmap(QPixmap(":/smartHome/smartHome/吊灯-关.png"));
            if(socket != nullptr){
                socket->write("1:客厅:吊灯:0");
            }
            ui->btnLamp->setText("吊灯开");
        }
    });

    //卧室按钮
    connect(ui->btnAir_1 , &QPushButton::clicked , this , [=](){
        //首先需要将图片改了
        if(ui->btnAir_1->text() == "空调开"){
            //首先需要将图片改了
            ui->labelAir_1->setPixmap(QPixmap(":/smartHome/smartHome/空调-开.png"));
            if(socket != nullptr){
                socket->write("1:主卧:空调:1");
            }
            ui->btnAir_1->setText("空调关");
        }else if (ui->btnAir_1->text() == "空调关") {
            ui->labelAir_1->setPixmap(QPixmap(":/smartHome/smartHome/空调-关.png"));
            if(socket != nullptr){
                socket->write("1:主卧:空调:0");
            }
            ui->btnAir_1->setText("空调开");
        }
    });
    connect(ui->btnLamp_1 , &QPushButton::clicked , this , [=](){
        if(ui->btnLamp_1->text() == "吊灯开"){
            ui->labelLamp_1->setPixmap(QPixmap(":/smartHome/smartHome/吊灯-开.png"));
            if(socket != nullptr){
                socket->write("1:主卧:吊灯:1");
            }
            ui->btnLamp_1->setText("吊灯关");
        }else if (ui->btnLamp_1->text() == "吊灯关") {
            ui->labelLamp_1->setPixmap(QPixmap(":/smartHome/smartHome/吊灯-关.png"));
            if(socket != nullptr){
                socket->write("1:主卧:吊灯:0");
            }
            ui->btnLamp_1->setText("吊灯开");
        }
    });

    // 餐厅按钮
    connect(ui->btnLamp_2 , &QPushButton::clicked , this , [=](){
        if(ui->btnLamp_2->text() == "吊灯开"){
            ui->labelLamp_2->setPixmap(QPixmap(":/smartHome/smartHome/吊灯-开.png"));
            if(socket != nullptr){
                socket->write("1:餐厅:吊灯:1");
            }
            ui->btnLamp_2->setText("吊灯关");
        }else if (ui->btnLamp_2->text() == "吊灯关") {
            ui->labelLamp_2->setPixmap(QPixmap(":/smartHome/smartHome/吊灯-关.png"));
            if(socket != nullptr){
                socket->write("1:餐厅:吊灯:0");
            }
            ui->btnLamp_2->setText("吊灯开");
        }
    });
    connect(ui->btnFan , &QPushButton::clicked , this , [=](){
        if(ui->btnFan->text() == "风扇开"){
            ui->labelFan->setPixmap(QPixmap(":/smartHome/smartHome/风扇-开.png"));
            if(socket != nullptr){
                socket->write("1:餐厅:风扇:1");
            }
            ui->btnFan->setText("风扇关");
        }else if(ui->btnFan->text() == "风扇关"){
            ui->labelFan->setPixmap(QPixmap(":/smartHome/smartHome/风扇-关.png"));
            if(socket != nullptr){
                socket->write("1:餐厅:风扇:0");
            }
            ui->btnFan->setText("风扇开");
        }
    });



    connect(ui->btnLamp_3 , &QPushButton::clicked , this , [=](){
        if(ui->btnLamp_3->text() == "吊灯开"){
            ui->labelLamp_3->setPixmap(QPixmap(":/smartHome/smartHome/吊灯-开.png"));
            if(socket != nullptr){
                socket->write("1:厨房:吊灯:1");
            }
            ui->btnLamp_3->setText("吊灯关");
        }else if (ui->btnLamp_3->text() == "吊灯关") {
            ui->labelLamp_3->setPixmap(QPixmap(":/smartHome/smartHome/吊灯-关.png"));
            if(socket != nullptr){
                socket->write("1:厨房:吊灯:0");
            }
            ui->btnLamp_3->setText("吊灯开");
        }
    });
    connect(ui->btnLamp_4 , &QPushButton::clicked , this , [=](){
        if(ui->btnLamp_4->text() == "吊灯开"){
            ui->labelLamp_4->setPixmap(QPixmap(":/smartHome/smartHome/吊灯-开.png"));
            if(socket != nullptr){
                socket->write("1:卫生间:吊灯:1");
            }
            ui->btnLamp_4->setText("吊灯关");
        }else if (ui->btnLamp_4->text() == "吊灯关") {
            ui->labelLamp_4->setPixmap(QPixmap(":/smartHome/smartHome/吊灯-关.png"));
            if(socket != nullptr){
                socket->write("1:卫生间:吊灯:0");
            }
            ui->btnLamp_4->setText("吊灯开");
        }
    });
}

SmartHome::~SmartHome()
{
    qDebug()<<"智能家居模块被析构了";
    delete service;
    delete ui;
}

void SmartHome::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(rect(),QPixmap(":/smartHome/smartHome/智能家居平面图.png"),QRect());
}

void SmartHome::closeEvent(QCloseEvent *)
{
    //当退出时，服务器直接断开连接
    if(socket != nullptr){
        socket->close();
        socket = nullptr;
    }
    service->close();
    ui->btnStart->setEnabled(true);
    emit over();
    this->close();
}

