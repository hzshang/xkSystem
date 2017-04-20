#include "hostdialog.h"
#include "ui_hostdialog.h"
#include <QMessageBox>
#include <sock.h>

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
    QString host = ui->hostLine->text();
    int port = ui->portLine->text().toInt();
    this->hide();
    emit showLogin(host,port);
}

void hostDialog::on_CancelButton_clicked()
{
    this->hide();
    QString host = "";
    int port = 0;
    emit showLogin(host,port);
}
