#include "forgetpwd.h"
#include "ui_forgetpwd.h"
#include <QDebug>
#include <QMessageBox>
#include <QPushButton>

ForgetPwd::ForgetPwd(QMap<int , User> map , QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ForgetPwd)
{
    this->forgetMap = map;
    ui->setupUi(this);
    this->setWindowTitle("忘记密码");
    connect(ui->btn1 , &QPushButton::clicked , this , [=](){
        //首先获取文本上的值
        int uid = ui->lineUser->text().toInt();
        long phone = ui->linePhone->text().toLong();
        //我们已经获取到了map值
        QMap<int , User>::iterator it;
        for(it = forgetMap.begin() ; it != forgetMap.end() ; it++){
            if(uid == it.key()){
                if(it.value().getPhone() == phone){
                    QString message = "找到账号,密码为:"+it.value().getPasswd();
                   int a = QMessageBox::information(this , "提示",message);
                   if(a == 1024){
                       this->close();
                       return ;
                   }
                }
                QMessageBox::information(this , "提示","信息校验失败");
                this->close();
                return;
            }
        }
        QMessageBox::information(this , "提示","没有找到账号");


    });
    connect(ui->btn2 , &QPushButton::clicked , this , &ForgetPwd::close);
}

ForgetPwd::~ForgetPwd()
{
    qDebug()<<"忘记密码被析构了";
    delete ui;
}

void ForgetPwd::setForgetMap(const QMap<int, User> &value)
{
    forgetMap = value;
}
