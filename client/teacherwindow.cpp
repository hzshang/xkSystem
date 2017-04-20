#include "teacherwindow.h"
#include "ui_teacherwindow.h"
#include "sock.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMap>
extern sock mysock;

teacherWindow::teacherWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::teacherWindow)
{
    ui->setupUi(this);
    this->setFixedSize( this->width(),this->height());
    ui->coursesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QObject::connect(ui->cNameComboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(setNumLine()));
}

teacherWindow::~teacherWindow()
{
    delete ui;
}

void teacherWindow::setCoursesTable(int &row, QString &cName, QString &cNum, QString &cCredit,
                                    QString &cTime, QString &cRoom, QString &curNum, QString &maxNum)//设置教师课表信息
{
    ui->coursesTable->setEnabled(true);

    QTableWidgetItem *itemName = new QTableWidgetItem(cName);
    ui->coursesTable->setItem(row,0,itemName);
    itemName->setTextAlignment(Qt::AlignHCenter);
    QTableWidgetItem *itemCNum = new QTableWidgetItem(cNum);
    ui->coursesTable->setItem(row,1,itemCNum);
    itemCNum->setTextAlignment(Qt::AlignHCenter);
    QTableWidgetItem *itemCredit = new QTableWidgetItem(cCredit);
    ui->coursesTable->setItem(row,2,itemCredit);
    itemCredit->setTextAlignment(Qt::AlignHCenter);
    QTableWidgetItem *itemTime = new QTableWidgetItem(cTime);
    ui->coursesTable->setItem(row,3,itemTime);
    itemTime->setTextAlignment(Qt::AlignHCenter);
    QTableWidgetItem *itemRoom = new QTableWidgetItem(cRoom);
    ui->coursesTable->setItem(row,4,itemRoom);
    itemRoom->setTextAlignment(Qt::AlignHCenter);
    QString str = curNum+"/"+maxNum;
    QTableWidgetItem *itemNum  = new QTableWidgetItem(str);
    ui->coursesTable->setItem(row,5,itemNum);
    itemNum->setTextAlignment(Qt::AlignHCenter);

}

void teacherWindow::getBasicInfo(QString &usr)//通过用户名请求教师基本信息，pid ＝ 13
{
    QJsonObject sendData;
    sendData.insert("pid",13);
    sendData.insert("user",usr);
    mysock.send(sendData);

    QJsonDocument recvData = mysock.recv();
    QJsonObject data = recvData.object();
    ui->nameLabel->setText(data["name"].toString());
    ui->departmentLabel->setText(data["dname"].toString());
}

QString teacherWindow::timeSwitch(QString &t)//将时间代码转换为文字时间
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

void teacherWindow::getOpenCourses(QString &usr)//通过用户名请求教师开设课程信息，pid = 11
{
    QJsonObject sendData;
    sendData.insert("pid",11);
    sendData.insert("user",usr);
    mysock.send(sendData);

    QJsonDocument recvData = mysock.recv();
    QJsonArray data = recvData.array();
    map.clear();
    ui->coursesTable->clear();
    QStringList listHeaders;
    listHeaders<<"课程名"<<"课程代码"<<"学分"<<"时间"<<"教师"<<"已选/上限";
    ui->coursesTable->setHorizontalHeaderLabels(listHeaders);
    int length = data.size();
    for(int i = 0;i < length;i++)
    {
        QJsonObject object = data.at(i).toObject();
        //qDebug()<<object;
        QString cName = object["cname"].toString();
        QString cNum = object["cid"].toString();
        map.insert(cName,cNum);
        int a = object["credit"].toInt();
        QString cCredit = QString("%1").arg(a);
        QString time = object["ctime"].toString();
        QString cTime = timeSwitch(time);
        QString cRoom = object["rid"].toString();
        int b = object["current"].toInt();
        QString curNum = QString("%1").arg(b);
        int c = object["max"].toInt();
        QString maxNum = QString("%1").arg(c);
        setCoursesTable(i,cName,cNum,cCredit,cTime,cRoom,curNum,maxNum);
    }
}
void teacherWindow::setComboxs()//设置cNamecombox以及
{
    int index = 0;
    QMap<QString, QString>::const_iterator i;
    for(i = map.constBegin(); i != map.constEnd(); ++i)
    {
        ui->cNameComboBox->insertItem(index,i.key());
        index++;
    }
}

//void teacherWindow::on_setPhotoPushButton_clicked()//上传头像
//{
//    localPhoto = QFileDialog::getOpenFileName(this);
//    if(!localPhoto.isEmpty())
//    {
//        QPixmap *pixmap = NULL;
//        pixmap = new QPixmap(95,105);
//        pixmap->load(localPhoto);
//        ui->photo->clear();
//        ui->photo->setPixmap(*pixmap);

//        if(pixmap)
//            ui->setPhotoPushButton->setText("更换图片");
//    }
//    else
//    {
//        ui->photo->setText(" Your Photo!");
//        ui->setPhotoPushButton->setText("上传图片");
//    }
//}

void teacherWindow::receiveLogin4(QString usr)//登陆加载界面
{
    this->show();
    userName = usr;
    ui->usrNameLable->setText(userName);
    getBasicInfo(userName);
    getOpenCourses(userName);
    setComboxs();
}

void teacherWindow::receiveAddCourse(QString usr)//从添加课程界面返回
{
    this->show();
    userName = usr;
    ui->usrNameLable->setText(userName);
    getBasicInfo(userName);
    getOpenCourses(userName);
    setComboxs();
}
void teacherWindow::receiveCourseInfo(QString usr)//从查看课程信息界面返回
{
    this->show();
    userName = usr;
    ui->usrNameLable->setText(userName);
    getBasicInfo(userName);
    getOpenCourses(userName);
    setComboxs();
}

void teacherWindow::deleteCourse()
{
    QJsonObject sendData;
    sendData.insert("pid",10);
    sendData.insert("cid",ui->cNumline->text());
    ui->cNameComboBox->clear();
    ui->cNumline->clear();
    mysock.send(sendData);

    QJsonDocument recvData = mysock.recv();
    QJsonObject data = recvData.object();
    bool state = data["state"].toBool();
    if(state)
    {
        QMessageBox::information(this,"信息","删除课程成功！",QMessageBox::Ok);
        getOpenCourses(userName);
        setComboxs();
    }
    else
    {
        QMessageBox::information(this,"信息","删除课程失败！",QMessageBox::Ok);
        ui->cNameComboBox->setCurrentIndex(0);
        return;
    }
}

void teacherWindow::setNumLine()//使combox和line能够信息一致,即课程名和课程代码相对应
{
    if(ui->cNameComboBox->currentText().isEmpty())
        return;
    QString str = ui->cNameComboBox->currentText();
    QMap<QString, QString>::iterator it;
    it = map.find(str);
    if(it != map.end())
        ui->cNumline->setText(it.value());
}

void teacherWindow::on_addCourseButton_clicked()
{
    ui->cNameComboBox->clear();
    ui->cNumline->clear();
    this->hide();
    emit addCourseRequest(userName);
}

void teacherWindow::on_deleteCourseButton_clicked()
{
    deleteCourse();
}

void teacherWindow::on_courseChosenButton_clicked()
{
    QString tName = ui->nameLabel->text();
    QString cName = ui->cNameComboBox->currentText();
    QString cNum = ui->cNumline->text();
    ui->cNameComboBox->clear();
    ui->cNumline->clear();
    this->hide();
    emit showCourseInfo(userName,tName,cName,cNum);
}

void teacherWindow::on_signOutButton_clicked()
{
    ui->cNameComboBox->clear();
    ui->cNumline->clear();
    this->hide();
    emit signOut();
}
