#ifndef LOADPAGE_H
#define LOADPAGE_H

#include <QWidget>

namespace Ui {
class LoadPage;
}

class LoadPage : public QWidget
{
    Q_OBJECT

public:
    explicit LoadPage(QWidget *parent = nullptr);
    ~LoadPage();
    int value;
signals:
    //取消登陆信号
    void disable();
    //完成信号
    void complete();
    //开始信号
    void start();

private:
    Ui::LoadPage *ui;
};

#endif // LOADPAGE_H
