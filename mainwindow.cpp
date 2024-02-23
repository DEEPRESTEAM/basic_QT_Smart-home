#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fileFunc.h"
#include <QDebug>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //在这里读取初始文件，用来开局显示勾选
    //首先读取用户数据用于显示账号密码
    QFile file("../service/file/tempUser.txt");
    bool isok = file.open(QIODevice::ReadOnly);
    if(isok == true){
        //可以写入文件
        QTextStream in(&file);
        in>>uid>>pwd;

    }else {
        qDebug()<<"打开文件失败";
    }
    file.close();
    //然后将权限读取到本地
    QFile fileFlag("../service/file/tempFlag.txt");
    bool isokFlag = fileFlag.open(QIODevice::ReadOnly);
    if(isokFlag == true){
        //可以写入文件
        QTextStream inFlag(&fileFlag);
        inFlag>>check1>>check2;
    }else {
        qDebug()<<"打开文件失败";
    }
    fileFlag.close();

    if(check2 == 1){
        ui->checkPasswd->setChecked(true);
        ui->lineUser->setText(QString().number(uid));
        ui->linePasswd->setText(pwd);
    }




    //初始设置
    //设置用户只能输入数字，不能输入其他的字符
    ui->lineUser->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));
    //设置用户账号长度与密码长度
    ui->lineUser->setMaxLength(6);
    ui->linePasswd->setMaxLength(16);
    //设置用户只能输入数字与字符的字符串
    ui->linePasswd->setValidator(new QRegExpValidator(QRegExp("[a-zA-z0-9]+$")));
    //设置密码隐藏
    ui->linePasswd->setEchoMode(QLineEdit::Password);

    //首先初始化所有的指针
    readUserFile(map);
    this->regist = new Regist(map);
    regist->hide();


    this->fdw = new ForgetPwd(regist->getMap());
    fdw->hide();

    this->ldp = new LoadPage();
    ldp->hide();

    //this->show();


    if(check1 == 1){
        ui->checkLogin->setChecked(true);
        this->hide();
        Login();
    }

    //首先实现注册页面
    connect(ui->btnRegist , &QPushButton::clicked , this , [=](){
        regist->setWindowModality(Qt::ApplicationModal);
        regist->show();
    });


    //实现忘记密码的绑定
    connect(ui->btnForget , &QPushButton::clicked , this , [=](){
        fdw->setForgetMap(regist->getMap());
        fdw->setWindowModality(Qt::ApplicationModal);
        fdw->show();
    });
    //实现登录的绑定
    connect(ui->btnLogin , &QPushButton::clicked , this , &MainWindow::Login);


    //实现自动登录与记住密码的复选
    connect(ui->checkLogin, &QCheckBox::stateChanged, this, [=](int state){
        if (2 == state) {
            ui->checkPasswd->setCheckState(Qt::Checked);
        }
    });
    connect(ui->checkPasswd, &QCheckBox::stateChanged, this, [=](int state){
        if (0 == state) {
            ui->checkLogin->setCheckState(Qt::Unchecked);
        }
    });
    connect(ldp , &LoadPage::disable , this , [=](){
        this->show();
        ldp->close();
    });
}

MainWindow::~MainWindow()
{
    delete fdw;
    delete regist;
    delete ldp;
    delete ui;
}

void MainWindow::Login()
{
    //首先判断是否填写内容
    if(ui->lineUser->text().isEmpty() || ui->linePasswd->text().isEmpty()){
        QMessageBox::information(this , "提示" , "请输入账号密码");
        return;
    }
    QMap<int , User>::iterator it;
    int uid = ui->lineUser->text().toInt();
    QString pwd = ui->linePasswd->text();
    for(it = regist->getMap().begin() ; it != regist->getMap().end() ; it++){
        if(it.key() == uid){
            //账号符合
            if(it.value().getPasswd() == pwd){
                //密码符合
                //将当前设置保存到文件中用于设置权限
                 if(ui->checkLogin->isChecked() == true && ui->checkPasswd->isChecked() == true){
                     writeFlag(uid , pwd , 1, 1);
                 }else if (ui->checkLogin->isChecked() == false && ui->checkPasswd->isChecked() == true) {
                     writeFlag(uid , pwd , 0, 1);
                 }else {
                     writeFlag(uid , pwd ,0 ,0);
                 }
                 QMessageBox::information( nullptr, "提示" , "登录成功");
                this->hide();
                 ldp->show();

                 return;
            }else{
                QMessageBox::information(this , "提示" , "密码错误");
                return;
            }

        }
    }
    QMessageBox::information(this , "提示" , "用户不存在");
}

void MainWindow::forgetPwd()
{

}

