                                                                                                                                                                                                                                                                                                         #include "regist.h"
#include "ui_regist.h"
#include "fileFunc.h"
#include "user.h"
#include <QDebug>


Regist::Regist(QMap<int, User> &map,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Regist)
{

    this->map = map;
    ui->setupUi(this);
    this->setWindowTitle("注册");
    //设置用户输入的信息长度
    ui->lineUser->setMaxLength(6);
    ui->linePasswd->setMaxLength(16);
    ui->linePhone->setMaxLength(15);
    //设置用户只能输入数字，不能输入其他的字符
    ui->lineUser->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));
    //设置用户只能输入数字与字符的字符串
    ui->linePasswd->setValidator(new QRegExpValidator(QRegExp("[a-zA-z0-9]+$")));
    ui->linePasswd_1->setValidator(new QRegExpValidator(QRegExp("[a-zA-z0-9]+$")));
    ui->linePhone->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));
    connect(ui->btnReturn , &QPushButton::clicked , this , &Regist::close);

    //设置密码隐藏
    ui->linePasswd->setEchoMode(QLineEdit::Password);
    ui->linePasswd_1->setEchoMode(QLineEdit::Password);
    //当点击注册按钮时，将文本栏里的值填写到文件中去，然后提示关闭窗口，然后返回登录页面
    connect(ui->btnReg , &QPushButton::clicked , this ,&Regist::registUser);
}

Regist::~Regist()
{
    qDebug()<<"注册模块被析构了";
    delete ui;
}

QMap<int, User>& Regist::getMap()
{
    return map;
}

void Regist::registUser()
{

    //首先查看同一uid是否多次注册
    if(map.contains(ui->lineUser->text().toInt())){
        //如果存在则提示该账号已被注册
        QMessageBox::information(this , "提示" , "该账号已被注册");
        return;
    }
    //查看状态栏里的内容是否为空
    if(ui->lineUser->text().isEmpty() || ui->linePasswd->text().isEmpty() || ui->linePasswd_1->text().isEmpty()){
        QMessageBox::information(this , "提示" , "请输入有效信息");
        return;
    }

    if(ui->linePasswd->text() != ui->linePasswd_1->text()){
        //说明两次密码不一致
        QMessageBox::information(this , "提示" , "密码不一致");
        return;
    }

    //获取，文本框里面的文件
    User user;
    user.setUid(ui->lineUser->text().toInt());
    user.setPasswd(ui->linePasswd->text());
    user.setPhone(ui->linePhone->text().toInt());
    //然后将数据加入到map中
    map.insert(ui->lineUser->text().toInt() , user);
    writeUserFile(user);

    //注册完后将账号密码信息置为空
    ui->lineUser->setText("");
    ui->linePhone->setText("");
    ui->linePasswd->setText("");
    ui->linePasswd_1->setText("");
    QMessageBox::information(this , "注册账号" ,"注册成功");

}
