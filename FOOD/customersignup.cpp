#include "customersignup.h"
#include "ui_customersignup.h"

customersignup::customersignup(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::customersignup)
{
    ui->setupUi(this);

    clientSocket = new ClientSocketManager(this);
    clientSocket->connectToServer("127.0.0.1", 1234); // آدرس و پورت سرور

    connect(clientSocket, &ClientSocketManager::messageReceived, this, [=](const QString &msg) {
        if (msg.startsWith("SIGNUP_OK:Customer")) {
            QMessageBox::information(this, "موفقیت", "ثبت نام با موفقیت انجام شد.");
            this->close();
        } else if (msg.startsWith("SIGNUP_FAIL:Customer")) {
            QMessageBox::warning(this, "خطا", "ثبت نام ناموفق بود، دوباره تلاش کنید.");
        }
    });

    connect(clientSocket, &ClientSocketManager::errorOccurred, this, [=](const QString &err) {
        QMessageBox::critical(this, "خطا", "ارتباط با سرور برقرار نشد: " + err);
    });
}

customersignup::~customersignup()
{
    delete ui;
}

void customersignup::on_pushButton_clicked()
{
    QString firstName = ui->lineEditFirstName->text().trimmed();
    QString lastName = ui->lineEditLastName->text().trimmed();
    QString phone = ui->lineEditPhone->text().trimmed();
    QString password = ui->lineEditPassword->text();
    QString passwordRepeat = ui->lineEditPasswordRepeat->text();

    if (password != passwordRepeat) {
        QMessageBox::warning(this, "خطا", "رمز عبور و تکرار آن یکسان نیستند!");
        return;
    }

    if (firstName.isEmpty() || lastName.isEmpty() || phone.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "خطا", "لطفاً همه فیلدها را پر کنید.");
        return;
    }

    // ساخت پیام ثبت‌نام طبق پروتکل سرور
    QString msg = QString("SIGNUP:Customer:%1:%2:%3:%4")
                      .arg(firstName)
                      .arg(lastName)
                      .arg(phone)
                      .arg(password);

    clientSocket->sendMessage(msg);
}
