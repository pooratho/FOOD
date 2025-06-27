#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include "database.h"
#include "customersignup.h"
#include "restaurantownersignupwindow.h"
#include "signinwindow.h"
namespace Ui {
class Form;
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
    Ui::Form *ui;
};

#endif // LOGINWINDOW_H
