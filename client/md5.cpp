#include "md5.h"
#include<QString>
#include <QCryptographicHash>

QString md5(QString pwd)
{
    return QString("%1").arg(QString(QCryptographicHash::hash(pwd.toUtf8(),QCryptographicHash::Md5).toHex())).mid(5,15);
}
