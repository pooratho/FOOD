#include "signinwindow.h"
#include "ui_signinwindow.h"
#include "loginwindow.h"
#include "adminmainpage.h"

SignInWindow::SignInWindow(LoginWindow *loginWin, const QString& role, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignInWindow),
    loginWindow(loginWin),
    selectedRole(role)
{
    ui->setupUi(this);

    clientSocket = new ClientSocketManager(this);
    clientSocket->connectToServer("127.0.0.1", 1234);  // آدرس و پورت سرور

    // اینجا کانکت رو به صورت مستقیم به تابع هندل مسیج وصل کن
    connect(clientSocket, &ClientSocketManager::messageReceived,
            this, &SignInWindow::handleServerMessage);

    connect(clientSocket, &ClientSocketManager::errorOccurred, this, [=](const QString &err) {
        QMessageBox::critical(this, "خطای اتصال", err);
    });
}

SignInWindow::~SignInWindow()
{
    delete ui;
}
void SignInWindow::on_pushButton_clicked()
{
    QString firstName = ui->lineEditFirstName->text().trimmed();
    QString lastName = ui->lineEditLastName->text().trimmed();
    QString password = ui->lineEditPassword->text().trimmed();

    if (firstName.isEmpty() || lastName.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "خطا", "لطفاً همه فیلدها را پر کنید.");
        return;
    }

    cachedFirstName = firstName;
    cachedLastName = lastName;
    cachedPassword = password;

    QString msg = "LOGIN:" + selectedRole + ":" + firstName + ":" + lastName + ":" + password;
    clientSocket->sendMessage(msg);
}

void SignInWindow::handleServerMessage(const QString &msg)
{
    if (msg.startsWith("LOGIN_OK:")) {
        QStringList parts = msg.split(":");
        if (parts.size() >= 2) {
            QString role = parts[1].trimmed();

            if (role.toLower() == "restaurant" && parts.size() >= 3) {
                QString restaurantName = parts[2].trimmed();

                // ساخت شی رستوران با نام واقعی که از سرور گرفته شده
                Restaurant restaurant(restaurantName, "Tehran", "Tehran"); // سایر فیلدها رو به دلخواه بزار
                RestaurantOwner* owner = new RestaurantOwner(
                    cachedFirstName,
                    cachedLastName,
                    cachedPhone,       // فرض کن این متغیرها ذخیره شده‌اند
                    cachedPassword,
                    restaurant
                    );

                // باز کردن صفحه اصلی رستوران
                RestaurantOwnerMainPage* page = new RestaurantOwnerMainPage(owner);
                page->setAttribute(Qt::WA_DeleteOnClose);
                page->show();

                this->close(); // بستن صفحه ورود
            }
            else if (role.toLower() == "customer" && parts.size() >= 3) {
                QString phone = parts[2].trimmed();

                QMessageBox::information(this, "ورود موفق", "خوش آمدید مشتری عزیز!");
                Customer* customer = new Customer(cachedFirstName, cachedLastName, phone, cachedPassword);
                CustomerMainPage* page = new CustomerMainPage(customer);
                page->setAttribute(Qt::WA_DeleteOnClose);
                page->show();

                this->close();
            }

            else if (role.toLower() == "admin") {
                QMessageBox::information(this, "ورود موفق", "خوش آمدید مدیر سیستم!");
                AdminMainPage* page = new AdminMainPage();
                page->setAttribute(Qt::WA_DeleteOnClose);
                page->show();

                this->close();
            }
            else {
                QMessageBox::warning(this, "خطا", "نقش دریافتی نامعتبر است.");
            }
        }
    }
    else if (msg.startsWith("LOGIN_FAIL:")) {
        QString reason = msg.section(':', 1);
        QMessageBox::warning(this, "ورود ناموفق", reason);
    }
}
