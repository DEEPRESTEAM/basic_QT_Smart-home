#include "user.h"

User::User()
{

}

QString User::getName() const
{
    return name;
}

void User::setName(const QString &value)
{
    name = value;
}

long User::getPhone() const
{
    return phone;
}

void User::setPhone(long value)
{
    phone = value;
}

QString User::getPasswd() const
{
    return passwd;
}

void User::setPasswd(const QString &value)
{
    passwd = value;
}

int User::getUid() const
{
    return uid;
}

void User::setUid(int value)
{
    uid = value;
}
