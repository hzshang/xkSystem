#include "logindialog.h"
#include "registerwindow.h"
#include "resetpwdwindow.h"
#include "studentwindow.h"
#include "teacherwindow.h"
#include "addcoursewindow.h"
#include "courseinfowindow.h"
#include "hostdialog.h"
#include "sock.h"
#include <QApplication>

sock mysock;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //mysock.init();
    loginDialog log;
    registerWindow reg;
    resetPwdWindow ret;
    studentWindow stu;
    teacherWindow tea;
    addCourseWindow add;
    courseInfoWindow cou;
    hostDialog hos;

    log.show();

    QObject::connect(&log,SIGNAL(showRegister()),&reg,SLOT(receiveLogin1()));
    QObject::connect(&log,SIGNAL(showResetPwd()),&ret,SLOT(receiveLogin2()));
    QObject::connect(&reg,SIGNAL(showLogin1()),&log,SLOT(receiveRegister()));
    QObject::connect(&ret,SIGNAL(showLogin2()),&log,SLOT(receiveResetPwd()));
    QObject::connect(&log,SIGNAL(showStuInfo(QString)),&stu,SLOT(receiveLogin3(QString)));
    QObject::connect(&log,SIGNAL(showTeaInfo(QString)),&tea,SLOT(receiveLogin4(QString)));
    QObject::connect(&reg,SIGNAL(registerDone(QString)),&log,SLOT(onRegisterDone(QString)));
    QObject::connect(&ret,SIGNAL(resetPwdDone(QString)),&log,SLOT(onResetPwdDone(QString)));
    QObject::connect(&tea,SIGNAL(addCourseRequest(QString)),&add,SLOT(receiveTeacherAddCourseRequest(QString)));
    QObject::connect(&add,SIGNAL(showTeacherWindow(QString)),&tea,SLOT(receiveAddCourse(QString)));
    QObject::connect(&tea,SIGNAL(showCourseInfo(QString,QString,QString,QString)),&cou,
                     SLOT(receiveTeacherInfoRequeset(QString,QString,QString,QString)));
    QObject::connect(&cou,SIGNAL(showTeacherWindow(QString)),&tea,SLOT(receiveCourseInfo(QString)));
    QObject::connect(&stu,SIGNAL(singOut()),&log,SLOT(receiveStudentInfo()));
    QObject::connect(&tea,SIGNAL(signOut()),&log,SLOT(receiveTeacherInfo()));
    QObject::connect(&hos,SIGNAL(showLogin(QString,int)),&log,SLOT(receiveIPSet(QString,int)));
    QObject::connect(&log,SIGNAL(showIPSet()),&hos,SLOT(receriveLogin()));
    return a.exec();

}
