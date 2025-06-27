#include "customersignup.h"
#include "ui_customersignup.h"

customersignup::customersignup(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::customersignup)
{
    ui->setupUi(this);
}

customersignup::~customersignup()
{
    delete ui;
}

void customersignup::on_pushButton_clicked()
{

        QString firstName = ui->lineEditFirstName->text();
        QString lastName = ui->lineEditLastName->text();
        QString phone = ui->lineEditPhone->text();
        QString password = ui->lineEditPassword->text();
        QString passwordRepeat = ui->lineEditPasswordRepeat->text();

        if (password != passwordRepeat) {
            QMessageBox::warning(this, "Error", "رمز عبور و تکرار آن یکسان نیستند!");
            return;
        }

        if (firstName.isEmpty() || lastName.isEmpty() || phone.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(this, "Error", "لطفا همه فیلدها را پر کنید.");
            return;
        }

        if (!dbManager.insertCustomer(firstName, lastName, phone, password)) {
            QMessageBox::warning(this, "Error", "ثبت اطلاعات انجام نشد.");
        } else {
            QMessageBox::information(this, "Success", "ثبت نام موفقیت‌آمیز بود!");
            // اینجا می‌تونی فرم رو پاک کنی یا به صفحه بعد بری
        }
    }

