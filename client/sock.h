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
    void init();
    void connect(QString host,int port);
    bool isConnected;

private:
    QTcpSocket mysocket;
};

#endif // SOCK_H
