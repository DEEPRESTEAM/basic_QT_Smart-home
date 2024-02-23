#ifndef FORGETPWD_H
#define FORGETPWD_H
#include "user.h"
#include <QMap>
#include <QWidget>

namespace Ui {
class ForgetPwd;
}

class ForgetPwd : public QWidget
{
    Q_OBJECT

public:
    explicit ForgetPwd(QMap<int , User> map , QWidget *parent = nullptr);
    ~ForgetPwd();
public:
    QMap<int , User> forgetMap;

    //返回注册后更新后的账号信息
    void setForgetMap(const QMap<int, User> &value);

private:
    Ui::ForgetPwd *ui;
};

#endif // FORGETPWD_H
