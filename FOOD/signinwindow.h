#ifndef SIGNINWINDOW_H
#define SIGNINWINDOW_H

#include <QWidget>
#include <QMessageBox>
#include "clientsocketmanager.h"
#include "customermainpage.h"

class LoginWindow;

namespace Ui {
class SignInWindow;
}

class SignInWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SignInWindow(LoginWindow *loginWin, QWidget *parent = nullptr);
    ~SignInWindow();

private slots:
    void on_pushButton_clicked();  // دکمه ورود

private:
    Ui::SignInWindow *ui;
    ClientSocketManager *clientSocket;
    LoginWindow *loginWindow;  // اشاره‌گر به پنجره اصلی
};

#endif // SIGNINWINDOW_H
