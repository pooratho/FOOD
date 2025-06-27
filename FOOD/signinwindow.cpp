#include "signinwindow.h"
#include "ui_signinwindow.h"

SignInWindow::SignInWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignInWindow)
{
    ui->setupUi(this);
}

SignInWindow::~SignInWindow()
{
    delete ui;
}

void SignInWindow::on_pushButton_clicked()
{
        QString firstName = ui->lineEditFirstName->text();
        QString lastName = ui->lineEditLastName->text();
        QString password = ui->lineEditPassword->text();

        if (firstName.isEmpty() || lastName.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(this, "خطا", "همه فیلدها را پر کنید.");
            return;
        }

        DatabaseManager::UserRole role = dbManager.checkUserLogin(firstName, lastName, password);

        switch (role) {
        case DatabaseManager::UserRole::Customer:
            QMessageBox::information(this, "ورود موفق", "خوش آمدید مشتری عزیز!");
          //  (new CustomerPage())->show();
            this->close();
            break;

        // case DatabaseManager::UserRole::Restaurant:
        //     QMessageBox::information(this, "ورود موفق", "ورود رستوران‌دار موفقیت‌آمیز بود.");
        //     (new RestaurantPage())->show();
        //     this->close();
        //     break;

        // case DatabaseManager::UserRole::Admin:
        //     QMessageBox::information(this, "ورود موفق", "خوش آمدید مدیر محترم!");
        //     (new AdminPage())->show();
        //     this->close();
        //     break;

        case DatabaseManager::UserRole::None:
        default:
            QMessageBox::warning(this, "خطا", "کاربری با این اطلاعات پیدا نشد.");
            break;
        }


}

