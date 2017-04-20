#include "resetpwdwindow.h"
#include "sock.h"
#include "md5.h"
#include "ui_resetpwdwindow.h"
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
extern sock mysock;

resetPwdWindow::resetPwdWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::resetPwdWindow)
{
    ui->setupUi(this);
    this->setFixedSize( this->width(),this->height());
}

resetPwdWindow::~resetPwdWindow()
{
    delete ui;
}

void resetPwdWindow::receiveLogin2()
{
    this->show();
}

void resetPwdWindow::on_resetPwdReturnPushButton_clicked()
{
    this->hide();
    emit showLogin2();
}

void resetPwdWindow::on_resetPwdConfirmPushButton_clicked()
{
    QString usrName = ui->resetPwdUsrNameLineEdit->text();
    if(usrName.isEmpty())
    {
        QMessageBox::warning(this,"警告","用名不得为空" ,QMessageBox::Ok);
        ui->resetPwdUsrNameLineEdit->clear();
        ui->resetPwdUsrNameLineEdit->setFocus();
        return;
    }

    QString name = ui->resetPwdNameLineEdit->text();
    if(name.isEmpty())
    {
        QMessageBox::warning(this,"警告","姓名不得为空" ,QMessageBox::Ok);
        ui->resetPwdNameLineEdit->clear();
        ui->resetPwdNameLineEdit->setFocus();
    }

    QString pwd = ui->resetPwdNewPwdLineEdit->text();
    if(pwd.isEmpty())
    {
        QMessageBox::warning(this,"警告","密码不能为空",QMessageBox::Ok);
        ui->resetPwdNewPwdLineEdit->clear();
        ui->resetPwdConfirmPwdLineEdit->clear();
        ui->resetPwdNewPwdLineEdit->setFocus();
    }
    int i = 0;
    int length = pwd.size();
    while(i < length)
    {
        if(pwd.mid(i,1) == " " )
        {
           QMessageBox::warning(this,"警告","密码不能含有空格",QMessageBox::Ok);
           ui->resetPwdNewPwdLineEdit->clear();
           ui->resetPwdConfirmPwdLineEdit->clear();
           ui->resetPwdNewPwdLineEdit->setFocus();
           return;
        }
        i++;
    }

    if(ui->resetPwdConfirmPushButton->text().trimmed() !=
            ui->resetPwdConfirmPushButton->text().trimmed())
    {
        QMessageBox box;
        box.setWindowTitle("警告");
        box.setIcon(QMessageBox::Warning);
        box.setText("密码不一致!");
        box.addButton(QMessageBox::Ok);
        box.exec();

        ui->resetPwdNewPwdLineEdit->clear();
        ui->resetPwdConfirmPwdLineEdit->clear();
        ui->resetPwdNewPwdLineEdit->setFocus();
        return;
    }

    QJsonObject send;
    send.insert("pid",3);
    send.insert("user",usrName);
    send.insert("name",name);
    QString t = md5(pwd);
    send.insert("pwd",t);
    mysock.send(send);

    QJsonDocument recvData=mysock.recv();
    bool state = recvData.object()["state"].toBool();
    if(!state)
    {
        QString out = "用户"+usrName+",修改密码失败！";
        QMessageBox::information(this,"消息",out,QMessageBox::Yes);

        ui->resetPwdUsrNameLineEdit->clear();
        ui->resetPwdNameLineEdit->clear();
        ui->resetPwdNewPwdLineEdit->clear();
        ui->resetPwdConfirmPwdLineEdit->clear();
        ui->resetPwdUsrNameLineEdit->setFocus();
        return;
    }
    QString out = "用户"+usrName+",修改密码成功！";
    QMessageBox::information(this,"消息",out,QMessageBox::Yes);
    this->hide();
    emit resetPwdDone(usrName);
}
