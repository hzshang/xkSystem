#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QMainWindow>
#include <QtNetwork>

namespace Ui {
class registerWindow;
}

class registerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit registerWindow(QWidget *parent = 0);
    ~registerWindow();

private slots:
    void identityIsTeacher();
    void receiveLogin1();

    void on_registerCancelPushButton_clicked();

    void on_registerConfirmPushButton_clicked();


signals:
    void registerDone(QString);
    void showLogin1();

private:
    Ui::registerWindow *ui;

    void setDepartment();




    //QComboBox *gradeComboBox;
};

#endif // REGISTERWINDOW_H
