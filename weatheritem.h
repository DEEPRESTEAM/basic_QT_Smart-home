#ifndef WEATHERITEM_H
#define WEATHERITEM_H

#include <QWidget>
#include <QNetworkAccessManager>//http 管理员
#include <QNetworkRequest>//http 请求
#include <QNetworkReply>
#include <QJsonArray>
namespace Ui {
class weatherItem;
}

class weatherItem : public QWidget
{
    Q_OBJECT
public:
    QMap<QString , QString> mapIcon;
    QMap<QString , QString> cityMap;
    QJsonArray  array;
public:
    explicit weatherItem(QWidget *parent = nullptr);
    ~weatherItem();
public:
    QNetworkAccessManager * manager;
    QNetworkRequest * request;
public:
    void weatherInformation(QNetworkReply *reply);
    void sendAPIMessage();
    void initCityCode();
    QString getCityCode(QString cityName);
    void preasJSON(QByteArray &byteArray);
    void initWeather(QString cityName);
    QString aqi(int num);

private:
    Ui::weatherItem *ui;
};

#endif // WEATHERITEM_H
