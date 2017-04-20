#include "courseinfowindow.h"
#include "ui_courseinfowindow.h"
#include "sock.h"
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
extern sock mysock;

courseInfoWindow::courseInfoWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::courseInfoWindow)
{
    ui->setupUi(this);
    this->setFixedSize( this->width(),this->height());
    ui->studentInfoTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

courseInfoWindow::~courseInfoWindow()
{
    delete ui;
}

void courseInfoWindow::setStudentInfoTable(int &row,QString &name, QString &num, QString &grade, QString &department)
{
    ui->studentInfoTable->setEnabled(true);

    QTableWidgetItem *itemName = new QTableWidgetItem(name);
    ui->studentInfoTable->setItem(row,0,itemName);
    itemName->setTextAlignment(Qt::AlignHCenter);
    QTableWidgetItem *itemNum = new QTableWidgetItem(num);
    ui->studentInfoTable->setItem(row,1,itemNum);
    itemNum->setTextAlignment(Qt::AlignHCenter);
    QTableWidgetItem *itemGrade = new QTableWidgetItem(grade);
    ui->studentInfoTable->setItem(row,2,itemGrade);
    itemGrade->setTextAlignment(Qt::AlignHCenter);
    QTableWidgetItem *itemdepartment = new QTableWidgetItem(department);
    ui->studentInfoTable->setItem(row,3,itemdepartment);
    itemdepartment->setTextAlignment(Qt::AlignHCenter);
}

void courseInfoWindow::getStudentInfoTable(QString &cid)
{
    QJsonObject sendData;
    sendData.insert("pid",12);
    sendData.insert("cid",cid);
    mysock.send(sendData);

    QJsonDocument recvData = mysock.recv();
    QJsonArray datas = recvData.array();
    ui->studentInfoTable->setEnabled(true);
    ui->studentInfoTable->clear();
    QStringList listHeaders;
    listHeaders<<"姓名"<<"学号"<<"年级"<<"学院";
    ui->studentInfoTable->setHorizontalHeaderLabels(listHeaders);
    int length = datas.size();
    for(int i = 0;i < length;i++)
    {
        QJsonObject object = datas.at(i).toObject();
        QString sName = object["name"].toString();
        QString sNum = object["id"].toString();
        int k = object["grade"].toInt();
        QString sGrade = "";
        switch (k) {
        case 1:
            sGrade += "大一";
            break;
        case 2:
            sGrade += "大二";
            break;
        case 3:
            sGrade += "大三";
            break;
        case 4:
            sGrade += "大四";
            break;
        default:
            break;
        }
        QString sDeparment = object["dname"].toString();

        setStudentInfoTable(i,sName,sNum,sGrade,sDeparment);
    }
}

void courseInfoWindow::receiveTeacherInfoRequeset(QString usr, QString name, QString cname, QString cid)
{
    this->show();
    QString tName = name;
    QString cName = cname;
    ui->welcomLabel->setText(tr("%1老师，你好！你开设的%2课选课信息如下：").arg(tName).arg(cName));
    userName = usr;
    cNum = cid;
    getStudentInfoTable(cNum);
}

void courseInfoWindow::on_confirmButton_clicked()
{
    this->hide();
    emit showTeacherWindow(userName);
}
