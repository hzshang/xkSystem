#ifndef ADDCOURSEWINDOW_H
#define ADDCOURSEWINDOW_H

#include <QMainWindow>

namespace Ui {
class addCourseWindow;
}

class addCourseWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit addCourseWindow(QWidget *parent = 0);
    ~addCourseWindow();

private slots:
    void receiveTeacherAddCourseRequest(QString);
    void setRoomTimeInfo();

    void on_cancelButton_clicked();

    void on_confirmButton_clicked();

signals:
    void showTeacherWindow(QString);

private:
    Ui::addCourseWindow *ui;
    QString userName;

    void addCourse();
    void getRoomInfo();
    QString timeSwitch(QString &t);
    QString timeSwitch2(QString &t);
};

#endif // ADDCOURSEWINDOW_H
