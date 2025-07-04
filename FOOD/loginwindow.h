#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include "customersignup.h"
#include "restaurantownersignup.h"
#include "signinwindow.h"
namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::LoginWindow *ui;
};

#endif // LOGINWINDOW_H
