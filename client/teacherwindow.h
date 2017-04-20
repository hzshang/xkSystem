#ifndef TEACHERWINDOW_H
#define TEACHERWINDOW_H

#include <QMainWindow>
#include <QMap>

namespace Ui {
class teacherWindow;
}

class teacherWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit teacherWindow(QWidget *parent = 0);
    ~teacherWindow();

private slots:
    void receiveLogin4(QString);

    void receiveAddCourse(QString);

    void receiveCourseInfo(QString);

    void setNumLine();

//    void on_setPhotoPushButton_clicked();

    void on_addCourseButton_clicked();

    void on_deleteCourseButton_clicked();

    void on_courseChosenButton_clicked();

    void on_signOutButton_clicked();

signals:
    void addCourseRequest(QString);
    void showCourseInfo(QString,QString,QString,QString);
    void signOut();

private:
    Ui::teacherWindow *ui;
    QString localPhoto;
    QString userName;
    QMap<QString,QString> map;

    void setCoursesTable(int &row,QString &cName,QString &cNum,QString &cCredit,
                         QString &cTime,QString &cRoom,QString &curNum,QString &maxNum);
    void getBasicInfo(QString &usr);
    void getOpenCourses(QString &usr);
    void deleteCourse();
    void setComboxs();
    QString timeSwitch(QString &t);

};

#endif // TEACHERWINDOW_H
