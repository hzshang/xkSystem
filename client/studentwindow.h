#ifndef STUDENTWINDOW_H
#define STUDENTWINDOW_H

#include <QMainWindow>

namespace Ui {
class studentWindow;
}

class studentWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit studentWindow(QWidget *parent = 0);
    ~studentWindow();

private slots:
//    void on_setPhotoPushButton_clicked();
    void receiveLogin3(QString);

    void on_nextPageButton_clicked();

    void on_lastPageButton_clicked();


    void on_chooseCourseButton_clicked();

    void on_dropCourseButton_clicked();


    void on_findCourseButton_clicked();

    void on_signOutButton_clicked();

signals:
    void singOut();
private:
    Ui::studentWindow *ui;
    QString localPhoto;
    QString userName;
    QString sendCName;
    QString sendTname;
    QString sendCId;
    int curPageNum;
    int maxPageNum;

    void setCourseChosenTable(int &row,int &col,QString &courseInfo);
    void setCoursesTable(int &row,QString &cName,QString &cNum,QString &cCredit,
                         QString &cTeacher,QString &cTime,QString &cRoom,QString &currentNum,QString &maxNum);
    void getBasicInfo(const QString &usr);
    void getCoursesChosenInfo(const QString &usr);
    void getCourses(int num,QString usr);
    void chooseCourse(QString &usr,QString &cNum);
    void dropCourse(QString &usr,QString &cNum);
    QString timeSwitch(QString &t);

};

#endif // STUDENTWINDOW_H
