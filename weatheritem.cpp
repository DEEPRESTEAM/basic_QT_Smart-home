#include "weatheritem.h"
#include "ui_weatheritem.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

weatherItem::weatherItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::weatherItem)
{
    ui->setupUi(this);
    //初始化指针对象
    this->manager = new QNetworkAccessManager(this);
    initWeather("长沙");
    connect(manager , &QNetworkAccessManager::finished , this , &weatherItem::weatherInformation);
    connect(ui->btnSoft , &QPushButton::clicked ,this , &weatherItem::sendAPIMessage);
    //首先将图片插入到map中
    mapIcon.insert("晴", ":/weather/weatherImages/type/Qing.png");
    mapIcon.insert("多云", ":/weather/weatherImages/type/DuoYun.png");
    mapIcon.insert("阴", ":/weather/weatherImages/type/Yin.png");
    mapIcon.insert("雨", ":/weather/weatherImages/type/Yu.png");
    mapIcon.insert("雪", ":/weather/weatherImages/type/Xue.png");
    mapIcon.insert("沙尘暴", ":/weather/weatherImages/type/ShaChenBao.png");

    mapIcon.insert("雷阵雨", ":/weather/weatherImages/type/LeiZhenYu.png");
    mapIcon.insert("大雨", ":/weather/weatherImages/type/DaYu.png");
    mapIcon.insert("小雨", ":/weather/weatherImages/type/XiaoYu.png");
    mapIcon.insert("中雨", ":/weather/weatherImages/type/ZhongYu.png");
    mapIcon.insert("阵雨", ":/weather/weatherImages/type/ZhenYu.png");
    mapIcon.insert("暴雨", ":/weather/weatherImages/type/BaoYu.png");
    mapIcon.insert("大暴雨", ":/weatherImages/type/DaBaoYu.png");
    mapIcon.insert("大到暴雨",":/weather/weatherImages/type/DaDaoBaoYu.png");
    mapIcon.insert("暴雨到大暴雨",":/weather/weatherImages/type/BaoYuDaoDaBaoYu.png");
    mapIcon.insert("大暴雨到大暴雨",":/weather/weatherImages/type/DaBaoYuDaoDaBaoYu.png");

    mapIcon.insert("暴雪",":/weather/weatherImages/type/BaoXue.png");
    mapIcon.insert("大到暴雪",":/weather/weatherImages/type/DaDaoBaoXue.png");
    mapIcon.insert("大雪", ":/weather/weatherImages/type/DaXue.png");
    mapIcon.insert("小雪", ":/weather/weatherImages/type/XiaoXue.png");
    mapIcon.insert("中雪", ":/weather/weatherImages/type/ZhongXue.png");

    mapIcon.insert("雨夹雪", ":/weather/weatherImages/type/YuJiaXue.png");
    mapIcon.insert("霾", ":/weather/weatherImages/type/Mai.png");
    mapIcon.insert("扬沙", ":/weather/weatherImages/type/YangSha.png");
    mapIcon.insert("沙尘暴", ":/weather/weatherImages/type/ShaChenBao.png");
    mapIcon.insert("特大暴雨", ":/weather/weatherImages/type/TeDaBaoYu.png");
    mapIcon.insert("乌", ":/weather/weatherImages/type/Wu.png");
    mapIcon.insert("小到中雨", ":/weather/weatherImages/type/XiaoDaoZhongYu.png");
    mapIcon.insert("小到中雪", ":/weather/weatherImages/type/XiaoDaoZhongXue.png");
    mapIcon.insert("雨夹雪", ":/weather/weatherImages/type/YuJiaXue.png");
    mapIcon.insert("阵雪", ":/weather/weatherImages/type/ZhenXue.png");
    ui->labelWeather->setAlignment(Qt::AlignCenter);
    ui->labelAir->setAlignment(Qt::AlignCenter);
    ui->labelLow->setAlignment(Qt::AlignCenter);
    ui->labelHight->setAlignment(Qt::AlignCenter);
    ui->labelWeather->setAlignment(Qt::AlignCenter);
    ui->labeldate->setAlignment(Qt::AlignCenter);
    ui->labelWind->setAlignment(Qt::AlignCenter);

    void (QComboBox::*itemIndex)(int) = &QComboBox::currentIndexChanged;
    connect(ui->comboBox , itemIndex , this , [=](int index){
        ui->labeldate->setText(array[index+1].toObject()["date"].toString());
        ui->labelWeather->setPixmap(QPixmap(mapIcon[array[index+1].toObject()["type"].toString()]));
        ui->labelAir->setText(aqi(array[index+1].toObject()["aqi"].toInt()));
        ui->labelLow->setText(array[index+1].toObject()["low"].toString().split(' ' , QString::SkipEmptyParts)[1]);
        ui->labelHight->setText(array[index+1].toObject()["high"].toString().split(' ' , QString::SkipEmptyParts)[1]);
        ui->labelWind->setText(array[index+1].toObject()["type"].toString());
    });


}

weatherItem::~weatherItem()
{
    qDebug()<<"天气模块被析构了";
    delete ui;
}

//当完成收到发送后处理
void weatherItem::weatherInformation(QNetworkReply *reply)
{
    // 响应的状态码为200，表示请求成功
    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    qDebug() << "operation: " << reply->operation();    //请求方式
    qDebug() << "status_code: " << status_code;         //状态码
    qDebug() << "url: " << reply->url();                //url
    qDebug() << "raw Header: " << reply->rawHeaderList(); //响应header

    if(reply->error() != QNetworkReply::NoError || status_code != 200)
    {
        //errorString()函数返回一个QString类型的错误字符串，而toLatin1()将该QString转换为使用Latin-1编码的QByteArray类型。
        //最后，data()函数返回QByteArray的数据指针
        qDebug() << reply->errorString().toLatin1().data();
        QMessageBox::warning(this,"提示","请求数据失败！",QMessageBox::Ok);
    }else{
        //获取响应信息
        QByteArray reply_data = reply->readAll();
        QByteArray byteArray = QString(reply_data).toUtf8();
        qDebug() << "read All: " << byteArray.data();

        //获取到json数据后进行解析
        preasJSON(byteArray);
    }

    //不然会造成内存泄漏
    reply->deleteLater();
}

void weatherItem::sendAPIMessage()
{
    //首先发送
    QString cityName = ui->lineEdit->text();

    //然后通过城市名字来获取code码
    QString cityCode = getCityCode(cityName);
    qDebug()<<cityCode;
    if(cityCode.isEmpty())
    {
       QMessageBox::warning(this,"错误","请检查是否输入正确的城市",QMessageBox::Ok);
       return;
    }
    QUrl url("http://t.weather.itboy.net/api/weather/city/" + cityCode);
    manager->get(QNetworkRequest(url)); //get请求完成就会是finished，上面的槽函数就会被调用
}

void weatherItem::initCityCode()
{
    //用于解析资源文件中获取城市码
    // 读取文件
    QFile file(":/weather/weatherImages/citycode.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // 处理文件打开失败的情况
        return;
    }
    QByteArray json = file.readAll();
    file.close();
     // 解析json，并且写入到map，这个文件是json数组
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(json, &error);
    if (error.error != QJsonParseError::NoError || !doc.isArray())
    {
        // 处理JSON解析失败的情况
        return;
    }
     // 然后获取json数据当中的城市名字和对应的编码
    QJsonArray cityArr = doc.array();
    for (int i = 0; i < cityArr.size(); i++)
    {
        QString city = cityArr[i].toObject().value("city_name").toString();
        QString code = cityArr[i].toObject().value("city_code").toString();
         // 然后写入到map
        // 注意当输入是省份时，是没有城市编码的，不可以查整个省份的天气，只能是具体的城市
        if (!code.isEmpty())
        {
            cityMap.insert(city, code);
        }
    }
}

QString weatherItem::getCityCode(QString cityName)
{
    // 初始化map
    if (cityMap.isEmpty())
    {
        initCityCode();
    }
     // 根据城市名字遍历map，获取城市编码
    QMap<QString, QString>::iterator it = cityMap.find(cityName);
    // 特殊：输入北京/北京市都可以找到
    if (it == cityMap.end())
    {
        it = cityMap.find(cityName + "市");
    }
    if (it == cityMap.end())
    {
        it = cityMap.find(cityName + "区");
    }
    if (it == cityMap.end())
    {
        it = cityMap.find(cityName + "县");
    }
     // 如果遍历到有这个城市名字，就返回编码
    if (it != cityMap.end())
    {
        return it.value();
    }
    return ""; // 没有就返回空
}

//用于解析Jeson
void weatherItem::preasJSON(QByteArray &byteArray)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(byteArray,&error); //如果报错就会把错误信息写到error里
    //报错直接退出
    if(error.error != QJsonParseError::NoError)
    {
        return;
    }
    //将json文档转化为json对象
    QJsonObject rootObj = doc.object();
    //获取当前城市信息
    QString city = rootObj["cityInfo"].toObject()["city"].toString();
    ui->labelCity->setText(city);
    //获取今天星期
    array = rootObj["data"].toObject()["forecast"].toArray();

    QString week = array[0].toObject()["week"].toString();
    //获取当期日期
    QString data = array[0].toObject()["ymd"].toString();
    //将当前日期与星期拼接在一起，放在一起
    QString dataAndWeek = week+' '+data;
    ui->labelData->setText(dataAndWeek);
    qDebug()<<dataAndWeek;

    //获取当前日期天气描述
    QString str = array[0].toObject()["type"].toString();
    ui->labelWether->setText(str);

    //获取当前日期的最高温度与最低温度
    QStringList lowlist  = array[0].toObject()["low"].toString().split(' ' , QString::SkipEmptyParts);
    QString low = lowlist[1];
    QStringList hightlist  = array[0].toObject()["high"].toString().split(' ' , QString::SkipEmptyParts);
    QString hight = hightlist[1];
    ui->labeTem->setText(low+'~'+hight);
    ui->labelHightTem->setText(rootObj["data"].toObject()["wendu"].toString());
    //图片填充
    ui->labelPix->setPixmap(QPixmap(mapIcon[str]));

    //设置下面默认是第二天的天气
    ui->labeldate->setText(array[1].toObject()["date"].toString());
    ui->labelWeather->setPixmap(QPixmap(mapIcon[array[1].toObject()["type"].toString()]));
    ui->labelAir->setText(aqi(array[1].toObject()["aqi"].toInt()));
    ui->labelLow->setText(array[1].toObject()["low"].toString().split(' ' , QString::SkipEmptyParts)[1]);
    ui->labelHight->setText(array[1].toObject()["high"].toString().split(' ' , QString::SkipEmptyParts)[1]);
    ui->labelWind->setText(array[1].toObject()["type"].toString());
    for(int i = 0 ; i < array.size() - 1 ; i++){
            ui->comboBox->setItemText(i , array[i+1].toObject()["date"].toString());
    }
}

void weatherItem::initWeather(QString cityName)
{
    //然后通过城市名字来获取code码
    QString cityCode = getCityCode(cityName);
    qDebug()<<cityCode;
    if(cityCode.isEmpty())
    {
       QMessageBox::warning(this,"错误","请检查是否输入正确的城市",QMessageBox::Ok);
       return;
    }
    QUrl url("http://t.weather.itboy.net/api/weather/city/" + cityCode);
    manager->get(QNetworkRequest(url)); //get请求完成就会是finished，上面的槽函数就会被调用
}

QString weatherItem::aqi(int num)
{
    if(num >0 && num <=50){
        return "优";
    }else if (num > 50 && num <= 100) {
        return "良";
    }else if (num > 100 && num <=150){
        return "轻度污染";
    }else if (num >150 && num <= 200) {
        return "中度污染";
    }else if (num >200 && num <= 250) {
        return "重度污染";
    }else {
        return "严重污染";
    }
}
