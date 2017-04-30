#ifndef COURSEINFOWINDOW_H
#define COURSEINFOWINDOW_H

#include <QMainWindow>

namespace Ui {
class courseInfoWindow;
}

class courseInfoWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit courseInfoWindow(QWidget *parent = 0);
    ~courseInfoWindow();
    void closeEvent(QCloseEvent *event);
private slots:
    void receiveTeacherInfoRequeset(QString,QString,QString,QString);

    void on_confirmButton_clicked();

signals:
    void showTeacherWindow(QString);

private:
    Ui::courseInfoWindow *ui;
    QString userName;
    QString cNum;
    //int curPageNum;
    //int maxPageNum;

    void getStudentInfoTable(QString &cid);
    void setStudentInfoTable(int &row,QString &name,QString &num,QString &grade,QString &department);
};

#endif // COURSEINFOWINDOW_H
