#ifndef SIGNINWINDOW_H
#define SIGNINWINDOW_H

#include <QWidget>
#include <QMessageBox>
#include "clientsocketmanager.h"

namespace Ui {
class SignInWindow;
}

class SignInWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SignInWindow(QWidget *parent = nullptr);
    ~SignInWindow();

private slots:
    void on_pushButton_clicked();  // دکمه ورود

private:
    Ui::SignInWindow *ui;
    ClientSocketManager *clientSocket;
};

#endif // SIGNINWINDOW_H
