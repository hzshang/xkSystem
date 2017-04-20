#ifndef HOSTDIALOG_H
#define HOSTDIALOG_H

#include <QDialog>

namespace Ui {
class hostDialog;
}

class hostDialog : public QDialog
{
    Q_OBJECT

public:
    explicit hostDialog(QWidget *parent = 0);
    ~hostDialog();

private slots:
    void on_confirmButton_clicked();
    void receriveLogin();

    void on_CancelButton_clicked();

signals:
    void showLogin(QString,int);
private:
    Ui::hostDialog *ui;
};

#endif // HOSTDIALOG_H
