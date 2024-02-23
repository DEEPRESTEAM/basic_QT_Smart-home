#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include "regist.h"
#include "forgetpwd.h"
#include "loadpage.h"
#include "centrecontroll.h"
namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT
public:
    int flag;
    Regist * regist;
    ForgetPwd * fdw;
    LoadPage * ldp;
    CentreControll * cent;
    int check1;
    int check2;
    int uid;
    QString pwd;
    QMap<int , User> map;
    //登录界面
    void Login();
    //忘记密码
    void forgetPwd();
    //中心界面
    void center();
public:
    explicit MyWidget(QWidget *parent = nullptr);
    ~MyWidget();
signals:
    //登录信号
    void log();
    //开始登录信号
    void loginStart();

private:
    Ui::MyWidget *ui;
};

#endif // MYWIDGET_H
