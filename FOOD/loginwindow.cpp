#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "mainwindow.h"
#include "clientsocketmanager.h"
#include <QMessageBox>
  // توجه: دقیقا همون اسمی که فایل داره

LoginWindow::LoginWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWindow)  // اصلاح شده
{
    ui->setupUi(this);
}


LoginWindow::~LoginWindow()
{
    delete ui;
}



void LoginWindow::on_pushButton_clicked()
{
    QString role = ui->comboBoxRole->currentText().trimmed();
    if (role.isEmpty()) {
        QMessageBox::warning(this, "خطا", "لطفاً یک نقش انتخاب کنید.");
        return;
    }

    // تبدیل نقش فارسی به انگلیسی
    QString roleEnglish;
    if (role == "مشتری") {
        roleEnglish = "Customer";
    } else if (role == "رستوران دار") {
        roleEnglish = "Restaurant";
    } else if (role == "مدیر سیستم") {
        roleEnglish = "Admin";
    } else {
        QMessageBox::warning(this, "خطا", "نقش انتخاب‌شده نامعتبر است.");
        return;
    }

    // باز کردن پنجره ورود بر اساس نقش انتخاب‌شده
    SignInWindow *signin = new SignInWindow(this, roleEnglish);
    signin->show();
}


void LoginWindow::on_pushButton_2_clicked()
{
    QString role = ui->comboBoxRole->currentText().trimmed();
    qDebug() << "نقش انتخاب‌شده:" << role;

    if (role == "مشتری") {
        customersignup* customerPage = new customersignup(this);
        customerPage->show();
    }
    else if (role == "رستوران دار") {
        RestaurantOwnerSignUp* restaurantPage = new RestaurantOwnerSignUp(this);
        restaurantPage->show();
    }
    else if (role == "مدیر سیستم") {
        QMessageBox::warning(this, "خطا", "برای مدیر سیستم، فقط گزینه ورود مجاز است.");
    }
    else {
        QMessageBox::warning(this, "خطا", "لطفاً یک نقش انتخاب کنید.");
    }
}


