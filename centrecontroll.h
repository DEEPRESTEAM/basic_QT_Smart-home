#ifndef CENTRECONTROLL_H
#define CENTRECONTROLL_H

#include <QWidget>
#include "musicitem.h"
#include "weatheritem.h"
#include "smarthome.h"
#include "monitoring.h"
namespace Ui {
class CentreControll;
}

class CentreControll : public QWidget
{
    Q_OBJECT
public:
    //音乐模块
    MusicItem * musicItem;
    //天气模块
    weatherItem * weather;
    //智能家居模块
    SmartHome * smartHomeItem;
    //监控模块
    Monitoring * monitoringItem;

public:
    explicit CentreControll(QWidget *parent = nullptr);
    ~CentreControll();
public:
    //重写关闭事件
    void closeEvent(QCloseEvent *event) override;
private:
    Ui::CentreControll *ui;
};

#endif // CENTRECONTROLL_H
