#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "regist.h"
#include "forgetpwd.h"
#include "loadpage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    int flag;
    Regist * regist;
    ForgetPwd * fdw;
    LoadPage * ldp;
    int check1;
    int check2;
    int uid;
    QString pwd;
    QMap<int , User> map;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void Login();
    void forgetPwd();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
