#ifndef SOCK_H
#define SOCK_H
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>

class sock
{
public:
    sock();
    bool send(const QJsonObject &);
    QJsonDocument recv();
    void connect(QString host,int port);

private:
    QTcpSocket mysocket;
    QString myhost;
    int myport;
};

#endif // SOCK_H
