#include "registerwindow.h"
#include "ui_registerwindow.h"
#include "sock.h"
#include "md5.h"
#include <QObject>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
extern sock mysock;

registerWindow::registerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::registerWindow)
{
    ui->setupUi(this);
    this->setFixedSize( this->width(),this->height());

    QStringList gradeItems  = {"大一","大二","大三","大四"};
    QStringList identityItems={"学生","教师"};
    ui->registerGradeComboBox->addItems(gradeItems);
    ui->registerIdentityComboBox->addItems(identityItems);
    connect(ui->registerIdentityComboBox,SIGNAL(currentTextChanged(QString)),
                     this,SLOT(identityIsTeacher()));

}

registerWindow::~registerWindow()
{
    delete ui;
}

void registerWindow::setDepartment()
{
    QJsonObject sendData;
    sendData.insert("pid",4);
    mysock.send(sendData);
    QJsonDocument recvData = mysock.recv();
    QJsonArray array = recvData.array();
    int length = array.size();
    for(int i = 0;i < length;i++)
        ui->registerDepartmentComboBox->insertItem(i,array[i].toString());
}

void registerWindow::identityIsTeacher()
{
    if(ui->registerIdentityComboBox->currentText() == "教师")
    {
        ui->registerGradeComboBox->addItem("");
        ui->registerGradeComboBox->setCurrentText("");
        ui->registerGradeComboBox->setEnabled(false);
    }
    else
    {
        ui->registerGradeComboBox->removeItem(4);
        ui->registerGradeComboBox->setCurrentIndex(0);
        ui->registerGradeComboBox->setEnabled(true);
    }
}

void registerWindow::receiveLogin1()
{
    this->show();
    setDepartment();
}

void registerWindow::on_registerCancelPushButton_clicked()
{
    this->hide();
    emit showLogin1();
}

void registerWindow::on_registerConfirmPushButton_clicked()
{
    if(ui->registerUsrNameLineEdit->text().isEmpty())
    {
        QMessageBox::warning(this,"警告","姓名不得为空" ,QMessageBox::Ok);
        ui->registerUsrNameLineEdit->clear();
        ui->registerUsrNameLineEdit->setFocus();
        return;
    }
    QString pwd = ui->registerPwdLineEdit->text();
    if(pwd.isEmpty())
    {
        QMessageBox::warning(this,"警告","密码不能为空",QMessageBox::Ok);
        ui->registerPwdLineEdit->clear();
        ui->registerPwdLineEdit->setFocus();
        return;
    }
    int i = 0;
    int length = ui->registerPwdLineEdit->text().size();
    while(i < length)
    {
        if(pwd.mid(i,1) == " " )
        {
           QMessageBox::warning(this,"警告","密码不能含有空格",QMessageBox::Ok);
           ui->registerPwdLineEdit->clear();
           ui->registerConfirmPwdLineEdit->clear();
           ui->registerPwdLineEdit->setFocus();
           return;
        }
        i++;
    }

    if(ui->registerPwdLineEdit->text().trimmed() !=
            ui->registerConfirmPwdLineEdit->text().trimmed())
    {
        QMessageBox box;
        box.setWindowTitle("警告");
        box.setIcon(QMessageBox::Warning);
        box.setText("密码不一致!");
        box.addButton(QMessageBox::Ok);
        box.exec();

        ui->registerPwdLineEdit->clear();
        ui->registerConfirmPwdLineEdit->clear();
        ui->registerPwdLineEdit->setFocus();
        return;
    }

    QJsonObject send;
    send.insert("pid",2);
    send.insert("name",ui->registerUsrNameLineEdit->text());
    QString t = md5(ui->registerPwdLineEdit->text());
    send.insert("pwd",t);
    send.insert("did",ui->registerDepartmentComboBox->currentText().mid(0,4));
    i = 0;
    if(ui->registerGradeComboBox->isEnabled())
        i = ui->registerGradeComboBox->currentIndex()+1;
    send.insert("grade",i);
    mysock.send(send);

    QJsonDocument recvData = mysock.recv();
    QString id = recvData.object()["user"].toString();
    QString out = "注册成功,用户id为:"+id;
    QMessageBox::information(NULL,"",out,QMessageBox::Yes);
    this->hide();
    emit registerDone(id);
}

