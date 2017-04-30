#include "addcoursewindow.h"
#include "ui_addcoursewindow.h"
#include "sock.h"
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
extern sock mysock;

addCourseWindow::addCourseWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::addCourseWindow)
{
    ui->setupUi(this);
    this->setFixedSize( this->width(),this->height());
    QObject::connect(ui->cRoomComboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(setRoomTimeInfo()));
}
void addCourseWindow::closeEvent(QCloseEvent *event)
{
    this->hide();
    emit showTeacherWindow(userName);
}

addCourseWindow::~addCourseWindow()
{
    delete ui;
}

QString addCourseWindow::timeSwitch2(QString &t)//中文课程时间转为时间代码
{
   QString time = "";
   if(t.contains("一",Qt::CaseSensitive))
       time += "1";
   else if(t.contains("二",Qt::CaseSensitive))
       time += "2";
   else if(t.contains("三",Qt::CaseSensitive))
       time += "3";
   else if(t.contains("四",Qt::CaseSensitive))
       time += "4";
   else if(t.contains("五",Qt::CaseSensitive))
       time += "5";
   else if(t.contains("六",Qt::CaseSensitive))
       time += "6";
   else if(t.contains("日",Qt::CaseSensitive))
       time += "7";
   int section = t.mid(4,1).toInt();
   switch (section) {
   case 1:
       time += "12";
       break;
   case 2:
       time += "22";
       break;
   case 3:
       time += "32";
       break;
   case 4:
       time += "42";
       break;
   case 5:
       time += "52";
       break;
   case 6:
       time += "62";
       break;
   case 7:
       time += "72";
       break;
   case 8:
       time += "82";
       break;
   case 9:
       time += "92";
       break;
   default:
       break;
   }
   return time;
}

void addCourseWindow::addCourse()
{
    QJsonObject sendData;
    sendData.insert("pid",9);
    sendData.insert("user",userName);
    sendData.insert("cname",ui->cNameLine->text());
    sendData.insert("rid",ui->cRoomComboBox->currentText());
    QString str = ui->cTimeComboBox->currentText();
    QString t = timeSwitch2(str);
    sendData.insert("ctime",t);
    sendData.insert("max",ui->cMaxNumLine->text().toInt());
    sendData.insert("credit",ui->cCreditLine->text().toInt());
    mysock.send(sendData);

    QJsonDocument recvData = mysock.recv();
    QJsonObject data = recvData.object();
    bool state = data["state"].toBool();
    if(!state)
    {
        QMessageBox::information(this,"信息","抱歉，添加课程失败！",QMessageBox::Ok);
        ui->cNameLine->clear();
        ui->cMaxNumLine->clear();
        ui->cCreditLine->clear();
        ui->cRoomComboBox->setCurrentIndex(0);
        ui->cNameLine->setFocus();
        return;
    }
    QString cid = data["cid"].toString();
    QMessageBox::information(this,"信息",tr("课程添加成功，课程id为%1").arg(cid),QMessageBox::Ok);
    ui->cNameLine->clear();
    ui->cMaxNumLine->clear();
    ui->cCreditLine->clear();
    ui->cRoomComboBox->clear();
    ui->cTimeComboBox->clear();
    ui->cNameLine->setFocus();
    this->hide();
    emit showTeacherWindow(userName);
}

void addCourseWindow::receiveTeacherAddCourseRequest(QString user)
{
    this->show();
    userName = user;
    getRoomInfo();
}

QString addCourseWindow::timeSwitch(QString &t)
{
    QString time = "";
    int day = t.mid(0,1).toInt();
    switch (day) {
    case 1:
        time += "星期一 ";
        break;
    case 2:
        time += "星期二 ";
        break;
    case 3:
        time += "星期三 ";
        break;
    case 4:
        time += "星期四 ";
        break;
    case 5:
        time += "星期五 ";
        break;
    case 6:
        time += "星期六 ";
        break;
    case 7:
        time += "星期日 ";
        break;
    default:
        break;
    }
    int section = t.mid(1,1).toInt();
    switch (section) {
    case 1:
        time += "1～2节";
        break;
    case 2:
        time += "2～3节";
        break;
    case 3:
        time += "3～4节";
        break;
    case 4:
        time += "4～5节";
        break;
    case 5:
        time += "5～6节";
        break;
    case 6:
        time += "6～7节";
        break;
    case 7:
        time += "7～8节";
        break;
    case 8:
        time += "8～9节";
        break;
    case 9:
        time += "9～10节";
        break;
    default:
        break;
    }
    return time;
}

void addCourseWindow::getRoomInfo()//获得所有教室的信息
{
    QJsonObject sendData;
    sendData.insert("pid",15);
    mysock.send(sendData);

    QJsonDocument recvData = mysock.recv();
    QJsonArray data = recvData.array();
    int length = data.size();
    for(int i = 0;i < length;i++)
    {
        ui->cRoomComboBox->insertItem(i,data[i].toString());
    }
}

void addCourseWindow::setRoomTimeInfo()//查看房间空余时间
{
    if(ui->cRoomComboBox->currentText().isEmpty())
            return;
    QJsonObject sendData;
    sendData.insert("pid",14);
    sendData.insert("rid",ui->cRoomComboBox->currentText());
    mysock.send(sendData);

    QJsonDocument recvData = mysock.recv();
    QJsonArray data = recvData.array();
    int length = data.size();
    int index = 0;
    ui->cTimeComboBox->clear();
    for(int i = 0;i < length;i++)
    {
        QString str = data[i].toString();
        if(str.mid(1,2) == "00")
        {
            QString s = tr("%1%2%3").arg(i+1).arg(1).arg(2);
            QString item = timeSwitch(s);
            ui->cTimeComboBox->insertItem(index++,item);
        }
        if(str.mid(2,2) == "00")
        {
            QString s = tr("%1%2%3").arg(i+1).arg(2).arg(3);
            QString item = timeSwitch(s);
            ui->cTimeComboBox->insertItem(index++,item);
        }
        if(str.mid(3,2) == "00")
        {
            QString s = tr("%1%2%3").arg(i+1).arg(3).arg(4);
            QString item = timeSwitch(s);
            ui->cTimeComboBox->insertItem(index++,item);
        }
        if(str.mid(4,2) == "00")
        {
            QString s = tr("%1%2%3").arg(i+1).arg(4).arg(5);
            QString item = timeSwitch(s);
            ui->cTimeComboBox->insertItem(index++,item);
        }
        if(str.mid(5,2) == "00")
        {
            QString s = tr("%1%2%3").arg(i+1).arg(5).arg(6);
            QString item = timeSwitch(s);
            ui->cTimeComboBox->insertItem(index++,item);
        }
        if(str.mid(6,2) == "00")
        {
            QString s = tr("%1%2%3").arg(i+1).arg(6).arg(7);
            QString item = timeSwitch(s);
            ui->cTimeComboBox->insertItem(index++,item);
        }
        if(str.mid(7,2) == "00")
        {
            QString s = tr("%1%2%3").arg(i+1).arg(7).arg(8);
            QString item = timeSwitch(s);
            ui->cTimeComboBox->insertItem(index++,item);
        }
        if(str.mid(8,2) == "00")
        {
            QString s = tr("%1%2%3").arg(i+1).arg(8).arg(9);
            QString item = timeSwitch(s);
            ui->cTimeComboBox->insertItem(index++,item);
        }
        if(str.mid(9,1) == "0")
        {
            QString s = tr("%1%2%3").arg(i+1).arg(9).arg(10);
            QString item = timeSwitch(s);
            ui->cTimeComboBox->insertItem(index++,item);
        }
    }
}

void addCourseWindow::on_cancelButton_clicked()
{
    this->hide();
    emit showTeacherWindow(userName);
}

void addCourseWindow::on_confirmButton_clicked()
{
    addCourse();
}
