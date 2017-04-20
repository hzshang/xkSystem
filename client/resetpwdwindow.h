#ifndef RESETPWDWINDOW_H
#define RESETPWDWINDOW_H

#include <QMainWindow>

namespace Ui {
class resetPwdWindow;
}

class resetPwdWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit resetPwdWindow(QWidget *parent = 0);
    ~resetPwdWindow();

private slots:
    void on_resetPwdReturnPushButton_clicked();
    void receiveLogin2();

    void on_resetPwdConfirmPushButton_clicked();

signals:
    void showLogin2();
    void resetPwdDone(QString);


private:
    Ui::resetPwdWindow *ui;

};

#endif // RESETPWDWINDOW_H
