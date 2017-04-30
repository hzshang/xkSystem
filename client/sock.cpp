#include "sock.h"
#include <QMessageBox>
sock::sock()
{
    return;
}

void sock::connect(QString host,int port)
{
    myhost=host;
    myport=port;
    if(mysocket.state()!=QAbstractSocket::UnconnectedState)
        return;
    mysocket.connectToHost(host,port);
    bool result = mysocket.waitForConnected(5000);
    if(!result)
    {
        QMessageBox::warning(NULL,"Warning","无法连接服务器",QMessageBox::Yes);
        return;
    }
}

bool sock::send(const QJsonObject &object)
{
   if(mysocket.state()==QAbstractSocket::UnconnectedState)
       connect(myhost,myport);
    QString str=QJsonDocument(object).toJson();
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
