#include "musicitem.h"
#include "ui_musicitem.h"

#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include "fileFunc.h"

MusicItem::MusicItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MusicItem)
{
    //现在还存在的问题就是，当我们再次放时，会清空map这时就没歌词了
    ui->setupUi(this);
    player = new QMediaPlayer(this);
    playList = new QMediaPlaylist(this);

    player->setPlaylist(playList);
    musicWritr = new QNetworkAccessManager(this);

    readMusicFile(musicMap);
    //将map的值设置到playlist与itemlist
    QMap<QString , QString>::iterator musicIt;
    for (musicIt = musicMap.begin() ; musicIt !=  musicMap.end(); musicIt++) {
        playList->addMedia(QMediaContent(QUrl(musicIt.key())));
        ui->listMusic->addItem(musicIt.value());
        qDebug()<<musicIt.key()<<" "<<musicIt.value();
    }


    clearMusicFile("","");

    //首先设置三个按钮
    connect(ui->btnPre , &QPushButton::clicked  , this , [=](){
        map.clear();
        ui->label_3->setText("暂无歌词");
        ui->label_2->setText("暂无歌词");
        playList->previous();
        int index = ui->listMusic->currentRow();
        if(index >= 0 && playList->media(index).canonicalUrl().toString().mid(0 , 4) == "http"){
            id = playList->media(index).canonicalUrl().toString().split("id=" ,QString::SkipEmptyParts)[1].split('.')[0].toInt();
            emit libretto();
        }
    });
    connect(ui->btnNext , &QPushButton::clicked , this , [=](){
        map.clear();
        ui->label_3->setText("暂无歌词");
        ui->label_2->setText("暂无歌词" );
        playList->next();

        int index = ui->listMusic->currentRow();
        if(index >= 0 && playList->media(index).canonicalUrl().toString().mid(0 , 4) == "http"){
            id = playList->media(index).canonicalUrl().toString().split("id=" ,QString::SkipEmptyParts)[1].split('.')[0].toInt();
            emit libretto();
        }

    });
    connect(ui->btnStart , &QPushButton::clicked , this , [=](){
        //首先开启音乐
        if(ui->btnStart->text()=="播放"){
            this->player->play();
            //然后将按钮置为暂停
            ui->btnStart->setText("暂停");
        }else if (ui->btnStart->text() == "暂停") {
            this->player->pause();
            ui->btnStart->setText("播放");
        }
    });

    //设置播放模式
    //首先设置默认值为顺序播放
    playList->setPlaybackMode(QMediaPlaylist::Sequential);
    //然后选择方法框时作为判断
    void (QComboBox::*comeboc)(int) = &QComboBox::currentIndexChanged;
    connect(ui->funBox ,comeboc , this , [=](int index){
        if(ui->funBox->itemText(index) == "单曲循环"){
            playList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        }else if(ui->funBox->itemText(index) == "随机播放"){
            playList->setPlaybackMode(QMediaPlaylist::Random);
        }else if(ui->funBox->itemText(index) == "列表循环"){
            playList->setPlaybackMode(QMediaPlaylist::Loop);
        }else if (ui->funBox->itemText(index) == "顺序播放") {
            playList->setPlaybackMode(QMediaPlaylist::Sequential);
        }
    });

    connect(player , &QMediaPlayer::durationChanged , this , [=](int duration){
        ui->horizontalSlider->setRange(0 , duration);
        QString timeEnd = QString("%1:%2").arg(duration/1000/60).arg(duration/1000%60);
        ui->labelEnd->setText(timeEnd);
    });
    //设置音量滑块
    //首先范围为
    ui->verticalSlider->setRange(0 , 100);
    //给个默认值为50;
    ui->verticalSlider->setValue(50);
    player->setVolume(50);
    connect(ui->verticalSlider , &QSlider::valueChanged , player , &QMediaPlayer::setVolume);



    //实现列表双击事件实现音乐播放
    connect(ui->listMusic , &QListWidget::itemDoubleClicked , this , [=](QListWidgetItem *item){
        //获取到列表中的索引
        map.clear();
        //需要区分是本地文件还是网络文件

        int index = ui->listMusic->row(item);
        if(playList->media(index).canonicalUrl().toString().mid(0 , 4) == "http"){
            id = playList->media(index).canonicalUrl().toString().split("id=" ,QString::SkipEmptyParts)[1].split('.')[0].toInt();
            emit libretto();
        }


        //将播放歌曲设置为当前行
        playList->setCurrentIndex(index);
        this->player->play();
        ui->btnStart->setText("暂停");
    });


    //将播放列表与实际播放跟随
    void (QListWidget::*selectIndex)(int) = &QListWidget::setCurrentRow;
    connect(playList , &QMediaPlaylist::currentIndexChanged , ui->listMusic, selectIndex);

    //实现添加音乐
    connect(ui->btnAdd , &QPushButton::clicked , this , [=](){
        QStringList musics = QFileDialog::getOpenFileNames(this , "选择音乐" ,"\\home");
        for(int i = 0 ; i < musics.size() ; i++){
            //将文件添加到playList中
            playList->addMedia(QMediaContent(QUrl::fromLocalFile(musics[i])));
            //将文件添加到ui列表
            QFileInfo info(musics[i]);
            ui->listMusic->addItem(info.fileName());
        }
    });

    //实现删除音乐
    connect(ui->btnDelete , &QPushButton::clicked , this , [=](){
        //首先将当前位置的歌曲从播放库里移除出去
        int index = ui->listMusic->currentRow();
        playList->removeMedia(index);
        ui->listMusic->takeItem(index);
    });
    player->setVideoOutput(ui->widget_7);

    //实现进度条与音乐滑块的变化
    connect(ui->horizontalSlider , &QSlider::sliderMoved , this , [=](qint64 pos){
        player->setPosition(pos);
    });

    //当歌曲位置发生变化时,设置底部播放时间
    connect(player , &QMediaPlayer::positionChanged , this , &MusicItem::playMusic);

    //下面就是通过网络来获取网络上的音乐
    this->http = new QNetworkAccessManager(this);

    connect(http , &QNetworkAccessManager::finished , this , &MusicItem::httpMessage);

    connect(ui->btnChat , &QPushButton::clicked , this , &MusicItem::softMessage);

    //用来解析歌词
    connect(musicWritr , &QNetworkAccessManager::finished , this , &MusicItem::musiceMessage);

    connect(this , &MusicItem::libretto , this , &MusicItem::librettoMessage);









}

MusicItem::~MusicItem()
{
    qDebug()<<"音乐模块被析构了";
    //获取列表中所有的音乐
    for (int j = 0; j < ui->listMusic->count(); j++)
    {
        //然后将这两组数据加入到文件中
        qDebug()<<playList->media(j).canonicalUrl().toString()<<" "<<ui->listMusic->item(j)->text();
        writeMusicFile( playList->media(j).canonicalUrl().toString() , ui->listMusic->item(j)->text());

    }

    delete ui;
}

void MusicItem::closeEvent(QCloseEvent * )
{
    player->pause();
    ui->btnStart->setText("播放");
    emit musicOver();
    this->hide();

}

void MusicItem::playMusic(qint64 value)
{

    ui->horizontalSlider->setValue(value);
    ui->labelStart->setText(QString("%1:%2").arg(value/1000/60).arg(value/1000%60));
    if(!map.isEmpty()){
        QMap<int , QString>::iterator it;
        for(it = map.begin() ; it != map.end() ; it++){
            if(player->position() >= it.key()){
                ui->label_2->setText(it.value());
                if((++it) == map.end()){
                    ui->label_3->setText("暂无歌词");
                    return;
                }else {
                    ui->label_3->setText(it.value());
                }

            }
        }
    }else {
        ui->label_2->setText("暂无歌词");
        ui->label_3->setText("暂无歌词");
    }

}

void MusicItem::httpMessage(QNetworkReply *reply)
{
    int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(code != 200)
    {
        QMessageBox::critical(this,"请求","请求出错，请检查网络连接状况");
        return;
    }
    //请求成功后 解析数据
    QByteArray data = reply->readAll();

    //解析数据第一步 将其转变为JSon 文档
    QJsonDocument docu = QJsonDocument::fromJson(data);
    //ui->textEdit->setText(docu.toJson());
    QJsonObject obj =  docu.object();
    if(obj.find("result") != obj.end())//代表找到了结果
    {
        if(obj["code"].toInt() == 400 || obj["result"].toObject()["songCount"].toInt() == 0 || obj["code"].toInt() == 406){
            return;
        }
        //obj["result"] 又是一个 json 对象
        obj = obj["result"].toObject();

        //obj["songs"] 为json 数组
        QJsonArray arry = obj["songs"].toArray();

        //我们只需要添加搜索到得第一首歌，arry 的第一个元素又是一个json 对象
        obj = arry[0].toObject();

        //检索音乐id
        id = obj["id"].toInt();

        musicTemp(id);
        //记得 我们如果要播放音乐 需要将链接放入到音乐列表
        //还有一个音乐播放的网址为:"http://music.163.com/song/media/outer/url?id=" + 音乐id;
        QString paly_adress = "http://music.163.com/song/media/outer/url?id=" + QString::number(id) + ".mp3";
        //如果没有后缀 将无法播放  会解码失败


        //将地址添加到 音乐播放列表
        this->playList->addMedia(QMediaContent(QUrl(paly_adress)));

        //还需要同步ui界面
        QString name = obj["name"].toString();
        ui->listMusic->addItem(name + ".mp3");
        //如果想要再次打开就有上一次的音乐列表时,我们需要将 name paly_adress 存放到音乐文件

    }
}

void MusicItem::softMessage()
{

    if(ui->line->text().isEmpty()){
        //如果是空的话，就返回就提示，直接返回
        QMessageBox::information(this , "提示" , "输入有效信息");
        return;
    }
    //发送请求  -> 必须要一个 链接 -> 去找免费的API 接口
    QString music_name = ui->line->text();
    QString request_adress = "http://music.163.com/api/search/get/web?csrf_token=hlpretag=&hlposttag=&s=" + music_name ;
    request_adress += "&type=1&offset=0&total=true&limit=2";

    //此链接支持get 方法
    this->http->get(QNetworkRequest(QUrl(request_adress)));
}


//当请求完成后实现的歌词浮现功能(这里不能是服务器相应就换歌词，而是要等到双击事件才行)
void MusicItem::musiceMessage(QNetworkReply *reply)
{
    map.clear();
    int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(code != 200)
    {
        QMessageBox::critical(this,"请求","请求出错，请检查网络连接状况");
        return;
    }
    QByteArray data = reply->readAll();
    //将data转换成Json文档
    QJsonDocument docu = QJsonDocument::fromJson(data);
    ui->textBrowser->setText(docu.toJson());
    QJsonObject obj = docu.object();
    QString arry = obj["lyric"].toString();
    QStringList list = arry.split('\n' , QString::SkipEmptyParts);
    for(int i = 0 ; i < list.size() ; i++){
        qDebug()<<list[i];
        qDebug()<<list[i].mid(0 , 11);
        qDebug()<<list[i].mid(11);
        //map.insert(list[i].mid(0 , 11) , list[i].mid(11));
        QRegularExpression regE("\\[(\\d+)?:(\\d+)?(\\.\\d+)?\\](.*)?");
        QRegularExpressionMatch match = regE.match(list[i].mid(0 , 11));
        if(match.hasMatch()){
            int h = match.captured(1).toInt() * 60000;
            int m = match.captured(2).toInt() * 1000;
            map.insert(h+m ,list[i].mid(11));
        }else {
            qDebug()<<"与字符串不匹配";
        }
    }



}

void MusicItem::librettoMessage()
{
    //发送请求  -> 必须要一个 链接 -> 去找免费的API 接口
    QString request_adress = "https://music.163.com/api/song/media?id="+QString().number(id);
    //此链接支持get 方法
    this->musicWritr->get(QNetworkRequest(QUrl(request_adress)));
}

