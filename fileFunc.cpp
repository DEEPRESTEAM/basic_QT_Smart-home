#include "fileFunc.h"
#include <QDataStream>
#include <QDebug>
#include <QTextCodec>



//读取用户文件
void readUserFile(QMap<int ,User> &map)
{
    QFile file("./Login.txt");
    bool isok = file.open(QIODevice::ReadOnly);
    if(isok == true){
        QTextStream in(&file);
        //如果返回为真，则读取文件,已文件流的形式读取文件
        while (!in.atEnd()) {
            User user;
            int uid;
            QString passwd;
            long phone;
            QString name;
            in>>uid>>passwd>>phone;
            //将数据存储到对象中
            user.setUid(uid);
            user.setPasswd(passwd.toUtf8().data());
            user.setPhone(phone);
            //然后以账号键值队的形式
            map.insert(uid , user);
        }
        //在这里还需要删除最后一行
        map.remove(0);
        file.close();
    }else{
        //打开文件失败;
        qDebug()<<"打开文件失败"<<'\n';
    }
}



//写入用户文件
void writeUserFile(User &user)
{
    QFile file("./Login.txt");
    bool isok = file.open(QIODevice::Append);
    if(isok == true){
        //可以写入文件
        QTextStream in(&file);
        in<<user.getUid()<<" "<<user.getPasswd().toUtf8().data()<<" "<<user.getPhone()<<'\n';
    }else {
        qDebug()<<"打开文件失败";
    }
    file.close();
}

//用于存放开始自动登录
void writeFlag(int uid, QString pwd, int login, int remember)
{
    //首先将用户数据写入到文件中
    QFile file("./tempUser.txt");
    bool isok = file.open(QIODevice::WriteOnly |QIODevice::Truncate);
    if(isok == true){
        //可以写入文件
        QTextStream in(&file);
        in<<uid<<" "<<pwd<<'\n';
    }else {
        qDebug()<<"打开文件失败";
    }
    file.close();
    //然后将权限写入到权限文件中
    QFile fileFlag(".tempFlag.txt");
    bool isokFlag = fileFlag.open(QIODevice::WriteOnly |QIODevice::Truncate);
    if(isokFlag == true){
        //可以写入文件
        QTextStream inFlag(&fileFlag);
        inFlag<<login<<" "<<remember;
    }else {
        qDebug()<<"打开文件失败";
    }
    fileFlag.close();
}


//存放进音乐文件中
void writeMusicFile(QString musicUrl , QString musicName){
    QFile file("./musicUrl.txt");

    bool isok = file.open(QIODevice::Append);
    if(isok == true){
        //可以写入文件
        QTextStream in(&file);
        if(file.size() == 0){
            in.seek(0);
        }
        musicUrl.replace(1 ,'\n');
        in<<musicUrl<<'|'<<musicName<<'\n';
    }else {
        qDebug()<<"打开文件失败";
    }
    file.close();
}

// 开始时清空音乐文件中
void clearMusicFile(QString musicUrl , QString musicName){
    QFile file("./musicUrl.txt");
    bool isok = file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    if(isok == true){
        //可以写入文件
        QTextStream in(&file);

        in<<musicUrl<<musicName;
    }else {
        qDebug()<<"打开文件失败";
    }
    file.close();
}

//将音乐文件读取出来,用于开始就加入音乐列表

void readMusicFile(QMap<QString , QString> &map)
{
    QFile file("./musicUrl.txt");
    bool isok = file.open(QIODevice::ReadOnly);
    if(isok == true){
        QTextStream in(&file);
        //如果返回为真，则读取文件,已文件流的形式读取文件

        while (!in.atEnd()) {
            QStringList list = in.readLine().split('|' ,QString::SkipEmptyParts);
            if(list.size() < 1){
                return;
            }else {
                map.insert(list[0] , list[1]);
            }
        }
        //在这里还需要删除最后一行
        map.remove(0);
        file.close();
    }else {
        //打开文件失败;
        qDebug()<<"打开文件失败"<<'\n';
    }
}


//存放进音乐文件中
void musicTemp(int id){
    QFile file("./temp.txt");

    bool isok = file.open(QIODevice::Append);
    if(isok == true){
        //可以写入文件
        QTextStream in(&file);
        if(file.size() == 0){
            in.seek(0);
        }

        in<<id<<'\n';
    }else {
        qDebug()<<"打开文件失败";
    }
    file.close();
}
