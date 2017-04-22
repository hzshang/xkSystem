#include "hostdialog.h"
#include "ui_hostdialog.h"
#include <QMessageBox>
#include <QSettings>
#include <QDebug>
hostDialog::hostDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::hostDialog)
{
    ui->setupUi(this);
}

hostDialog::~hostDialog()
{
    delete ui;
}

void hostDialog::receriveLogin()
{
    this->show();
}

void hostDialog::on_confirmButton_clicked()
{
    if(ui->hostLine->text().isEmpty())
    {
        QMessageBox::warning(this,"警告","IP地址不能为空",QMessageBox::Ok);
        return;
    }
    if(ui->portLine->text().isEmpty())
    {
        QMessageBox::warning(this,"警告","端口不能为空",QMessageBox::Ok);
        return;
    }
    this->hide();
    QString host = ui->hostLine->text();
    int port = ui->portLine->text().toInt();
    QSettings *s = new QSettings("server.ini",QSettings::IniFormat);
    //s->clear();
    tr("server.ini").resize(0);
    s->setValue("host",host);
    //qDebug()<<s->value("host").toString();
    s->setValue("port",port);
    emit showLogin();
}

void hostDialog::on_CancelButton_clicked()
{
    this->hide();
    //QString host = "";
    //int port = 0;
    emit showLogin();
}
