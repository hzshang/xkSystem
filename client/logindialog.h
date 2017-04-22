#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QList>
#include <QSettings>
#include <QFile>
#include <QStringList>
#include <QMap>

struct Login{
    QString userName;
    QString password;
};

namespace Ui {
class loginDialog;
}

class loginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit loginDialog(QWidget *parent = 0);
    ~loginDialog();

private slots:
    void onRegisterDone(QString);
    void onResetPwdDone(QString);
    void receiveRegister();
    void receiveResetPwd();
    void receiveStudentInfo();
    void receiveTeacherInfo();
    void receiveIPSet();
    void setPwd();

    void on_registerPushButton_clicked();

    void on_resetPwdPushButton_clicked();

    void on_loginPushButton_clicked();

    void on_rememberPwd_clicked();

    void on_setButton_clicked();

signals:
    void showRegister();
    void showResetPwd();
    void showStuInfo(QString);
    void showTeaInfo(QString);
    void showIPSet();


private:
    Ui::loginDialog *ui;
    bool rememberPwd = false;
    QList<Login> logins;
    QMap<QString,QString> map;
    QString myfile = "test.ini";
    QString file = "server.ini";
    QSettings *settings = new QSettings(myfile,QSettings::IniFormat);

    QString host = "";
    int port = 0;
    bool isConnected = false;

    void setUserInfo();
    void updateUserInfo();
    bool checkPwd();
    void connectToSever();
    void initServer();
};

#endif // LOGINDIALOG_H
