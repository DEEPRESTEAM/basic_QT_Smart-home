#ifndef USER_H
#define USER_H

#include <QObject>

class User
{

public:
    int uid;
    QString passwd;
    long phone;
    QString name;
    User();
    int getUid() const;
    void setUid(int value);

    QString getPasswd() const;
    void setPasswd(const QString &value);

    long getPhone() const;
    void setPhone(long value);

    QString getName() const;
    void setName(const QString &value);


};

#endif // USER_H
