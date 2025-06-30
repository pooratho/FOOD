#ifndef CUSTOMERSIGNUP_H
#define CUSTOMERSIGNUP_H


#include <QDialog>
#include "clientsocketmanager.h"
#include "customermainpage.h"
#include "customer.h"  // ⬅️ حتما اینو اضافه کن

class LoginWindow;

namespace Ui {
class customersignup;
}

class customersignup : public QDialog
{
    Q_OBJECT

public:
    explicit customersignup(LoginWindow *loginWin, QWidget *parent = nullptr);
    ~customersignup();

private slots:
    void on_pushButton_clicked();

private:
    Ui::customersignup *ui;
    ClientSocketManager *clientSocket;
    LoginWindow *loginWindow;

    // ⬅️ شی از کلاس Customer
    Customer* customer = nullptr;

    // برای ذخیره مقادیر اولیه قبل از ثبت‌نام موفق
    QString firstName, lastName, phone, password;
};

#endif // CUSTOMERSIGNUP_H
