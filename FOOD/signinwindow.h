#ifndef SIGNINWINDOW_H
#define SIGNINWINDOW_H

#include <QWidget>
#include <QMessageBox>
#include "clientsocketmanager.h"
#include "customermainpage.h"
#include "customer.h"

class LoginWindow;

namespace Ui {
class SignInWindow;
}

class SignInWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SignInWindow(LoginWindow *loginWin, const QString& role, QWidget *parent = nullptr);

    ~SignInWindow();

private slots:
    void on_pushButton_clicked();  // دکمه ورود

private:
    Ui::SignInWindow *ui;
    ClientSocketManager *clientSocket;
    LoginWindow *loginWindow;  // اشاره‌گر به پنجره اصلی
    QString selectedRole;
private:
    QString cachedFirstName;
    QString cachedLastName;
    QString cachedPassword;
    QString cachedPhone;

};

#endif // SIGNINWINDOW_H
