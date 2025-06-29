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

    // اتصال به پیام دریافتی از سرور
    connect(clientSocket, &ClientSocketManager::messageReceived, this, [=](const QString &msg) {
        if (msg.startsWith("LOGIN_OK:")) {
            QString role = msg.section(':', 1); // نقش رو جدا کن

            if (role == "Customer") {
                QMessageBox::information(this, "ورود موفق", "خوش آمدید مشتری عزیز!");

                CustomerMainPage *page = new CustomerMainPage();  // بدون parent
                page->setAttribute(Qt::WA_DeleteOnClose);         // با بستن آزاد شود
                page->show();                                     // باز کن

                loginWindow->close();  // فقط پنجره اصلی رو ببند
                this->close();
            }
            else if (role == "Restaurant") {
                QMessageBox::information(this, "ورود موفق", "خوش آمدید رستوران‌دار محترم!");

                RestaurantOwnerMainPage *page = new RestaurantOwnerMainPage();  // بدون parent
                page->setAttribute(Qt::WA_DeleteOnClose);         // با بستن آزاد شود
                page->show();                                     // باز کن

                loginWindow->close();  // فقط پنجره اصلی رو ببند
                this->close();
            }
        }
        else if (msg.startsWith("LOGIN_FAIL")) {
            QMessageBox::warning(this, "خطا", "ورود ناموفق! اطلاعات اشتباه است.");
        }
    });

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
        QMessageBox::warning(this, "خطا", "همه فیلدها را پر کنید.");
        return;
    }

    // بررسی محرمانه برای نقش Admin
    if (selectedRole == "Admin") {
        if (firstName != "ADMIN" || lastName != "ADMIN" || password != "ADMIN") {
            QMessageBox::critical(this, "خطا", "اطلاعات ورود مدیر سیستم نادرست است.");
            return;
        }

        // ورود موفق مدیر سیستم (بدون نیاز به ارسال به سرور)
        QMessageBox::information(this, "ورود موفق", "خوش آمدید مدیر محترم سیستم!");

        AdminMainPage* page = new AdminMainPage();  // فرض بر اینکه چنین صفحه‌ای وجود دارد
        page->setAttribute(Qt::WA_DeleteOnClose);
        page->show();

        loginWindow->close();
        this->close();
        return;
    }

    // برای سایر نقش‌ها: ارسال به سرور
    QString msg = "LOGIN:" + selectedRole + ":" + firstName + ":" + lastName + ":" + password;
    clientSocket->sendMessage(msg);
}

