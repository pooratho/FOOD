#include "loginwindow.h"
#include "customersignup.h"
#include "ui_login.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}



void LoginWindow::on_pushButton_clicked()
{
        QString role = ui->comboBoxRole->currentText();

    if (role == "مشتری") {
        SignInWindow* SignInPage = new SignInWindow();
        SignInPage ->show();
        this->close(); // یا این پنجره رو مخفی کن
    }
    else if (role == "رستوران دار") {
        SignInWindow* SignInPage = new SignInWindow();
        SignInPage ->show();
        this->close();
    }

    else {
        QMessageBox::warning(this, "خطا", "لطفاً یک نقش انتخاب کنید.");
    }

}


void LoginWindow::on_pushButton_2_clicked()
{
    QString role = ui->comboBoxRole->currentText();
    qDebug() << "نقش انتخاب‌شده:" << role;


        if (role == "مشتری") {
            customersignup* customerPage = new customersignup();
            customerPage ->show();
            this->close(); // یا این پنجره رو مخفی کن
        }
        // else if (role == "مدیر برنامه") {
        //     AdminPage* adminPage = new AdminPage();
        //     adminPage->show();
        //     this->close();
        // }
        else if (role == "رستوران دار") {
            RestaurantOwnerSignUpWindow* restaurantPage = new RestaurantOwnerSignUpWindow();
            restaurantPage->show();
            this->close();
        }
        else {
            QMessageBox::warning(this, "خطا", "لطفاً یک نقش انتخاب کنید.");
        }

}

