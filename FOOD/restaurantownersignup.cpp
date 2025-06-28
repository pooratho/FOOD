#include "restaurantownersignup.h"
#include "ui_restaurantownersignup.h"


RestaurantOwnerSignUp::RestaurantOwnerSignUp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RestaurantOwnerSignUp)
{
    ui->setupUi(this);

    // اتصال به سرور با استفاده از کلاس مدیریت سوکت
    clientSocket = new ClientSocketManager(this);
    clientSocket->connectToServer("127.0.0.1", 1234);
    // وقتی پاسخی از سرور گرفتیم
    connect(clientSocket, &ClientSocketManager::messageReceived, this, [=](const QString& response) {
        if (response.startsWith("SIGNUP_OK")) {
            QMessageBox::information(this, "موفق", "ثبت‌نام رستوران با موفقیت انجام شد.");
            this->close();
        } else {
            QMessageBox::warning(this, "خطا", "ثبت‌نام رستوران ناموفق بود.");
        }
    });
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
        QMessageBox::warning(this, "خطا", "رمز عبور و تکرار آن یکسان نیستند!");
        return;
    }

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

    // ساخت پیام برای ارسال به سرور
    QString msg = QString("SIGNUP_RESTAURANT:%1:%2:%3:%4:%5:%6:%7")
                      .arg(restaurantName)
                      .arg(ownerFirstName)
                      .arg(ownerLastName)
                      .arg(phone)
                      .arg(province)
                      .arg(city)
                      .arg(password);

    clientSocket->sendMessage(msg);
}
