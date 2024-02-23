#ifndef SMARTHOME_H
#define SMARTHOME_H

#include <QWidget>
#include <QPainter>
#include<QTcpServer>
#include<QTcpSocket>
#include <QTimer>

namespace Ui {
class SmartHome;
}

class SmartHome : public QWidget
{
    Q_OBJECT
public:
    QTcpServer *service;
    QTcpSocket *socket;
    QTimer * timer;
    int num;
public:
    explicit SmartHome(QWidget *parent = nullptr);
    ~SmartHome();
public:
    void paintEvent(QPaintEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
signals:
    void over();
    void add();

private:
    Ui::SmartHome *ui;
};

#endif // SMARTHOME_H
