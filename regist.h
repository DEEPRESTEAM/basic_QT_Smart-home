#ifndef REGIST_H
#define REGIST_H

#include <QWidget>
#include <QMessageBox>
#include <QMap>
#include "user.h"
namespace Ui {
class Regist;
}

class Regist : public QWidget
{
    Q_OBJECT

public:
    explicit Regist(QMap<int , User> &map , QWidget *parent = nullptr);
    ~Regist();
public:
    QMap<int , User> map;
    void registUser();

    QMap<int, User>& getMap();

private:
    Ui::Regist *ui;
};

#endif // REGIST_H
