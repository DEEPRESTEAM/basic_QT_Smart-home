#ifndef MUSICITEM_H
#define MUSICITEM_H

#include <QWidget>
#include <QSslSocket>
#include <QMediaPlayer>
#include <QMediaPlaylist>

#include <QNetworkReply>//http 处理
#include <QNetworkAccessManager>//http 管理员
#include <QNetworkRequest>//http 请求

#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include <QList>
namespace Ui {
class MusicItem;
}

class MusicItem : public QWidget
{
    Q_OBJECT

public:
    //首先播放音乐需要这两个类
    QMediaPlayer * player;
    QMediaPlaylist * playList;

    //实例化一个管理员,用于读取歌曲
    QNetworkAccessManager *http;

    //用于读取歌词的管理员
    QNetworkAccessManager *musicWritr;
    int id;

    QMap<int , QString> map;
    QMap<QString , QString> musicMap;

    int lyricsID;
    int temp;

    void playMusic(qint64 value);
    void httpMessage(QNetworkReply *reply);
    void softMessage();

    //当音乐管理者发送get请求后服务器发送消息
    void musiceMessage(QNetworkReply *reply);

    //用于管理员发送歌词接口
    void librettoMessage();

    explicit MusicItem(QWidget *parent = nullptr);
    ~MusicItem();
    void closeEvent(QCloseEvent * )override;
signals:
    //void closeMusic();
    void libretto();
    void musicOver();

private:
    Ui::MusicItem *ui;
};

#endif // MUSICITEM_H
