#include "studentwindow.h"
#include "ui_studentwindow.h"
#include "sock.h"
#include <QFileDialog>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTableWidget>
#include <QMessageBox>
extern sock mysock;

studentWindow::studentWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::studentWindow)
{
    ui->setupUi(this);
    this->setFixedSize( this->width(),this->height());
    ui->coursesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->courseChosenTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

studentWindow::~studentWindow()
{
    delete ui;
}

void studentWindow::getBasicInfo(const QString &usr)//通过用户名，获取学生的基本信息
{
    QJsonObject sendData;
    sendData.insert("pid",13);
    sendData.insert("user",usr);
    mysock.send(sendData);


    QJsonDocument recvData = mysock.recv();
    QJsonObject data = recvData.object();
    ui->nameLabel->setText(data["name"].toString());
    ui->departmentLabel->setText(data["dname"].toString());
    ui->roundLabel->setText(tr("当前为第%1轮选课时间").arg(data["round"].toInt()));
}

void studentWindow::setCourseChosenTable(int &row,int &col,QString &courseInfo)
{
    ui->courseChosenTable->setEnabled(true);

    QTableWidgetItem *cInfo = new QTableWidgetItem(courseInfo);
    cInfo->setBackgroundColor(QColor(0,191,255));
    ui->courseChosenTable->setItem(row,col,cInfo);
    //qDebug()<<ui->courseChosenTable->item(row,col)->data(Qt::DisplayRole).toString();
}

void studentWindow::setCoursesTable(int &row, QString &cName, QString &cNum, QString &cCredit,
                                    QString &cTeacher, QString &cTime, QString &cRoom, QString &currentNum, QString &maxNum)
{
    QTableWidgetItem *itemName = new QTableWidgetItem(cName);
    ui->coursesTable->setItem(row,0,itemName);
    itemName->setTextAlignment(Qt::AlignHCenter);
    QTableWidgetItem *itemNum = new QTableWidgetItem(cNum);
    ui->coursesTable->setItem(row,1,itemNum);
    itemNum->setTextAlignment(Qt::AlignHCenter);
    QTableWidgetItem *itemCredit = new QTableWidgetItem(cCredit);
    ui->coursesTable->setItem(row,2,itemCredit);
    itemCredit->setTextAlignment(Qt::AlignHCenter);
    QTableWidgetItem *itemTeacher = new QTableWidgetItem(cTeacher);
    ui->coursesTable->setItem(row,3,itemTeacher);
    itemTeacher->setTextAlignment(Qt::AlignHCenter);
    QTableWidgetItem *itemTime = new QTableWidgetItem(cTime);
    ui->coursesTable->setItem(row,4,itemTime);
    itemTime->setTextAlignment(Qt::AlignHCenter);
    QTableWidgetItem *itemRoom = new QTableWidgetItem(cRoom);
    ui->coursesTable->setItem(row,5,itemRoom);
    itemRoom->setTextAlignment(Qt::AlignHCenter);
    QTableWidgetItem *itemCurnum  = new QTableWidgetItem(currentNum);
    ui->coursesTable->setItem(row,6,itemCurnum);
    itemCurnum->setTextAlignment(Qt::AlignHCenter);
    QTableWidgetItem *itemMaxnum = new QTableWidgetItem(maxNum);
    ui->coursesTable->setItem(row,7,itemMaxnum);
    itemMaxnum->setTextAlignment(Qt::AlignHCenter);
}


void studentWindow::getCoursesChosenInfo(const QString &usr)//通过用户名，获取学生的选课情况
{
    QJsonObject sendData;
    sendData.insert("pid",5);
    sendData.insert("user",usr);
    mysock.send(sendData);

    QJsonDocument recvData = mysock.recv();
    QJsonArray data = recvData.array();
    int length = data.size();
    ui->courseChosenTable->setEnabled(true);
    ui->courseChosenTable->clear();
    ui->courseChosenTable->clearSpans();
    QStringList listHeaders;
    listHeaders<<"周一"<<"周二"<<"周三"<<"周四"<<"周五"<<"周六"<<"周日";
    ui->courseChosenTable->setHorizontalHeaderLabels(listHeaders);
    for(int i = 0;i < length;i++)
    {
        QJsonObject object = data.at(i).toObject();
        QString cTime = object["ctime"].toString();
        int col = cTime.mid(0,1).toInt() - 1;
        int row = cTime.mid(1,1).toInt() - 1;
        ui->courseChosenTable->setSpan(row,col,2,1);
        QString cName = object["cname"].toString();
        //qDebug()<<cName;
        QString cTeacher = object["tname"].toString();
        QString str = cName +"("+cTeacher+")";
        QString cNum = object["cid"].toString();
        QString cRoom = object["rid"].toString();
        str += "("+cNum+") ("+cRoom+")";
        setCourseChosenTable(row,col,str);
    }
}

QString studentWindow::timeSwitch(QString &t)//讲时间转为文字信息
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

void studentWindow::getCourses(int num,QString usr)//获取第num页的课程信息，pid = 6
{
    QJsonObject sendData;
    sendCName = ui->cNameLine->text();
    sendCId = ui->cNumLine->text();
    sendTname = ui->cTeacherLine->text();
    sendData.insert("pid",6);
    sendData.insert("index",num);
    sendData.insert("user",usr);
    sendData.insert("cname",sendCName);
    sendData.insert("cid",sendCId);
    sendData.insert("tname",sendTname);

    mysock.send(sendData);

    QJsonDocument recvData = mysock.recv();
    QJsonObject datas = recvData.object();

    maxPageNum = (datas["total"].toInt()-1)/10;
    QJsonArray data = datas["courses"].toArray();

    int length = data.size();
    ui->coursesTable->setEnabled(true);
    ui->coursesTable->clear();
    QStringList listHeaders;
    listHeaders<<"课程名"<<"课程代码"<<"教师"<<"学分"<<"时间"<<"教室"<<"已选人数"<<"人数上限";
    ui->coursesTable->setHorizontalHeaderLabels(listHeaders);
    for(int i = 0;i < length;i++)
    {
        QJsonObject object = data.at(i).toObject();
        QString cName = object["cname"].toString();
        QString cNum = object["cid"].toString();
        int a = object["credit"].toInt();
        QString cCredit = QString("%1").arg(a);
        QString cTeacher = object["tname"].toString();
        QString time = object["ctime"].toString();
        QString cTime = timeSwitch(time);
        QString cRoom = object["rid"].toString();
        int b = object["current"].toInt();
        QString curNum = QString("%1").arg(b);
        int c = object["max"].toInt();
        QString maxNum = QString("%1").arg(c);
        setCoursesTable(i,cName,cNum,cTeacher,cCredit,cTime,cRoom,curNum,maxNum);
    }
    int m = curPageNum+1;
    int n = maxPageNum+1;
    ui->pageInfoLabel->setText(tr("第%1页，共%2页").arg(m).arg(n));
}

void studentWindow::chooseCourse(QString &usr, QString &cNum)//学生选课，pid = 7
{
    QJsonObject sendData;
    sendData.insert("pid",7);
    sendData.insert("user",usr);
    sendData.insert("cid",cNum);
    mysock.send(sendData);

    QJsonDocument recvData = mysock.recv();
    QJsonObject data = recvData.object();
    bool state = data["state"].toBool();
    if(!state)
    {
        QMessageBox::information(this,"信息","选课失败",QMessageBox::Ok);
    }else
        QMessageBox::information(this,"信息","选课成功",QMessageBox::Ok);
    curPageNum = 0;
    getCourses(curPageNum,userName);
    getCoursesChosenInfo(usr);
}

void studentWindow::dropCourse(QString &usr, QString &cNum)//学生退课，pid = 8
{
    QJsonObject sendData;
    sendData.insert("pid",8);
    sendData.insert("user",usr);
    sendData.insert("cid",cNum);
    mysock.send(sendData);

    QJsonDocument recvData = mysock.recv();
    QJsonObject data = recvData.object();
    bool state = data["state"].toBool();
    if(!state)
    {
        QMessageBox::information(this,"信息","退课失败",QMessageBox::Ok);
        return;
    }
    QMessageBox::information(this,"信息","退课成功",QMessageBox::Ok);
    curPageNum = 0;
    getCourses(curPageNum,userName);
    getCoursesChosenInfo(usr);
}

void studentWindow::receiveLogin3(QString user)//加载界面
{
    this->show();
    userName = user;
    ui->userNameLabel->setText(userName);
    int grade = userName.mid(0,1).toInt();
    switch (grade) {
    case 1:
        ui->gradeLabel->setText("一年级");
        break;
    case 2:
        ui->gradeLabel->setText("二年级");
        break;
    case 3:
        ui->gradeLabel->setText("三年级");
        break;
    case 4:
        ui->gradeLabel->setText("四年级");
        break;
    default:
        break;
    }
    getBasicInfo(userName);

    curPageNum = 0;
    getCourses(curPageNum,userName);
    getCoursesChosenInfo(userName);
}

void studentWindow::clearLine()
{
    ui->cNameLine->clear();
    ui->cNumLine->clear();
    ui->cTeacherLine->clear();
}

void studentWindow::on_nextPageButton_clicked()
{
    if(curPageNum == maxPageNum)
    {
        QMessageBox::information(this,"信息","已经是最后一页！",QMessageBox::Ok);
        return;
    }
    curPageNum++;
    getCourses(curPageNum,userName);
}

void studentWindow::on_lastPageButton_clicked()
{
    if(curPageNum == 0)
    {
        QMessageBox::information(this,"信息","已经是第一页！",QMessageBox::Ok);
        return;
    }
    curPageNum--;
    sendCName = ui->cNameLine->text();
    sendCId = ui->cNumLine->text();
    sendTname = ui->cTeacherLine->text();
    getCourses(curPageNum,userName);
}

void studentWindow::on_chooseCourseButton_clicked()
{
    if(ui->cNumLine->text().isEmpty())
    {
        QMessageBox::warning(this,"警告","你需要输入课程号码！",QMessageBox::Ok);
        ui->cNumLine->setFocus();
        return;
    }
    QString str = ui->cNumLine->text();\
    clearLine();
    chooseCourse(userName,str);
}

void studentWindow::on_dropCourseButton_clicked()
{
    if(ui->cNumLine->text().isEmpty())
    {
        QMessageBox::warning(this,"警告","你需要输入课程号码！",QMessageBox::Ok);
        ui->cNumLine->setFocus();
        return;
    }
    QString str = ui->cNumLine->text();
    clearLine();
    dropCourse(userName,str);
}

void studentWindow::on_findCourseButton_clicked()
{
    curPageNum = 0;
    getCourses(curPageNum,userName);
}

void studentWindow::on_signOutButton_clicked()
{
    this->hide();
    emit singOut();
}

QString studentWindow::getData(int k)
{
    int curRow=ui->coursesTable->currentIndex().row();
    QAbstractItemModel *modessl = ui->coursesTable->model();
    QModelIndex indextemp = modessl->index(curRow,k);
    QString datatemp = modessl->data(indextemp).toString();
    return datatemp;
}


void studentWindow::on_coursesTable_itemClicked(QTableWidgetItem *item)
{
    //int curRow = item->row();
    ui->cNameLine->setText(getData(0));
    ui->cNumLine->setText(getData(1));
    ui->cTeacherLine->setText(getData(2));
}

void studentWindow::on_courseChosenTable_itemClicked(QTableWidgetItem *item)
{
    if(item == NULL)
    {
        clearLine();
        return;
    }
    QString info = item->text();
    int i,l = 0;
    int size = info.size();
    for(i = l;i < size;i++)
    {
        if(info.mid(i,1) == "(")
            break;
    }
    ui->cNameLine->setText(info.mid(0,i));
    l = i+1;
    for(i = l;i < size;i++)
    {
        if(info.mid(i,1) == ")")
            break;
    }
    ui->cTeacherLine->setText(info.mid(l,i-l));
    l = i+2;
    for(i = l;i < size;i++)
    {
        if(info.mid(i,1) == ")")
            break;
    }
    ui->cNumLine->setText(info.mid(l,i-l));
}
