#ifndef FILEFUNC_H
#define FILEFUNC_H
#include <QFile>
#include <QMap>
#include "user.h"
//首先这里提供关于文件操作

//读取用户文件
void readUserFile(QMap<int , User> &map);

//写入用户文件
void writeUserFile(User &user);

void musicTemp(int id);
//记住用户密码与登录状态
void writeFlag(int uid , QString pwd , int login , int remember);

//存放进音乐文件中
void writeMusicFile(QString musicUrl , QString musicName);

// 开始时清空音乐文件中
void clearMusicFile(QString musicUrl , QString musicName);

//存放进音乐文件中
void readMusicFile(QMap<QString , QString> &map);

#endif // FILEFUNC_H
