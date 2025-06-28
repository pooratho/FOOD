#include "restaurantownersignup.h"
#include "ui_restaurantownersignup.h"

    RestaurantOwnerSignUp::RestaurantOwnerSignUp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RestaurantOwnerSignUp)
{
    ui->setupUi(this);
}

RestaurantOwnerSignUp::~RestaurantOwnerSignUp()
{
    delete ui;
}

void RestaurantOwnerSignUp::on_pushButton_clicked()
{

        QString restaurantName = ui->lineEditRestaurantName->text();
        QString ownerFirstName = ui->lineEditOwnerFirstName->text();
        QString ownerLastName = ui->lineEditOwnerLastName->text();
        QString phone = ui->lineEditPhone->text();
        QString province = ui->comboBoxProvince->currentText();
        QString city = ui->comboBoxCity->currentText();
        QString password = ui->lineEditPassword->text();
        QString passwordRepeat = ui->lineEditPasswordrepeat->text();

        if (password != passwordRepeat) {
            QMessageBox::warning(this, "Error", "رمز عبور و تکرار آن یکسان نیستند!");
            return;
        }

        // چک خالی بودن فیلدها
        if (restaurantName.isEmpty() || ownerFirstName.isEmpty() || ownerLastName.isEmpty() || phone.isEmpty()
            || province.isEmpty() || city.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(this, "Error", "لطفا همه فیلدها را پر کنید.");
            return;
        }

        if (!dbManager.insertRestaurant(restaurantName, ownerFirstName, ownerLastName, phone, province, city, password)) {
            QMessageBox::warning(this, "Error", "ثبت اطلاعات انجام نشد.");
        } else {
            QMessageBox::information(this, "Success", "ثبت نام رستوران موفقیت‌آمیز بود!");
            this->close();

            if (this->parentWidget()) {
                this->parentWidget()->close();
            }
        }


}

