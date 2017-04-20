#include "sock.h"
#include <QMessageBox>
sock::sock()
{
    isConnected=false;
    return;
}
void sock::init()
{

    return;
}

void sock::connect(QString host,int port)
{
    if(isConnected)
        return;
    mysocket.connectToHost(host,port);
    bool result = mysocket.waitForConnected(5000);
    if(!result)
    {
        QMessageBox::warning(NULL,"Warning","无法连接服务器",QMessageBox::Yes);
        exit(0);
    }else
        isConnected=true;
}

bool sock::send(const QJsonObject &object)
{
    QString str=QJsonDocument(object).toJson();
    qWarning()<<str;
    QByteArray tosend=str.toUtf8();
    mysocket.write(tosend);
    return mysocket.waitForBytesWritten();
}


QJsonDocument sock::recv()
{
    mysocket.waitForReadyRead();
    QByteArray bytes=mysocket.readAll();
    return QJsonDocument::fromJson(bytes);//Parses json as a UTF-8
}
