#include "customersignup.h"
#include "ui_customersignup.h"
#include "loginwindow.h"
#include "customermainpage.h"

customersignup::customersignup(LoginWindow *loginWin, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::customersignup)
    , loginWindow(loginWin)
{
    ui->setupUi(this);

    clientSocket = new ClientSocketManager(this);
    clientSocket->connectToServer("127.0.0.1", 1234); // آدرس و پورت سرور

    // اتصال به پاسخ سرور
    connect(clientSocket, &ClientSocketManager::messageReceived, this, [=](const QString &msg) {
        if (msg.startsWith("SIGNUP_OK")) {
            // ✅ ساخت شی Customer بعد از موفقیت
            customer = new Customer(firstName, lastName, phone, password);

            // رفتن به صفحه اصلی مشتری و انتقال شی
            CustomerMainPage *page = new CustomerMainPage(customer); // ارسال شی به صفحه بعدی
            page->setAttribute(Qt::WA_DeleteOnClose);
            page->show();

            //loginWindow->close();  // بستن پنجره لاگین
            this->close();         // بستن فرم ثبت‌نام

        } else if (msg.startsWith("SIGNUP_FAIL")) {
            QMessageBox::warning(this, "خطا", "ثبت نام ناموفق بود، دوباره تلاش کنید.");
        }
    });

    // خطای اتصال به سرور
    connect(clientSocket, &ClientSocketManager::errorOccurred, this, [=](const QString &err) {
        QMessageBox::critical(this, "خطا", "ارتباط با سرور برقرار نشد: " + err);
    });
}

customersignup::~customersignup()
{
    delete ui;
    delete customer;  // پاک کردن شی ساخته‌شده برای جلوگیری از نشت حافظه
}

void customersignup::on_pushButton_clicked()
{
    // گرفتن اطلاعات از فرم
    firstName = ui->lineEditFirstName->text().trimmed();
    lastName = ui->lineEditLastName->text().trimmed();
    phone = ui->lineEditPhone->text().trimmed();
    password = ui->lineEditPassword->text();
    QString passwordRepeat = ui->lineEditPasswordRepeat->text();

    // بررسی صحت فیلدها
    if (password != passwordRepeat) {
        QMessageBox::warning(this, "خطا", "رمز عبور و تکرار آن یکسان نیستند!");
        return;
    }

    if (firstName.isEmpty() || lastName.isEmpty() || phone.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "خطا", "لطفاً همه فیلدها را پر کنید.");
        return;
    }

    // ارسال پیام ثبت‌نام به سرور
    QString msg = QString("SIGNUP_CUSTOMER:%1:%2:%3:%4")
                      .arg(firstName)
                      .arg(lastName)
                      .arg(phone)
                      .arg(password);

    clientSocket->sendMessage(msg);
}
