#include "adminmainpage.h"
#include "ui_adminmainpage.h"
#include "restauranttablewidget.h"

#include <QTableWidget>
#include <QMessageBox>

// ✅ هدر جدول رستوران‌ها

AdminMainPage::AdminMainPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AdminMainPage)
    , clientSocket(new ClientSocketManager(this))   // ⬅️ ابتدا سوکت
{
    ui->setupUi(this);
    setWindowTitle(u"پنل مدیریت سیستم"_qs);

    // ⬇️ اتصال دریافت پیام‌های سرور
    connect(clientSocket, &ClientSocketManager::messageReceived,
            this, &AdminMainPage::handleServerMessage);

    clientSocket->connectToServer("127.0.0.1", 1234);

    // ⬇️ اتصال دکمۀ «لیست رستوران‌ها»
    connect(ui->pushButton, &QPushButton::clicked, this, [this]() {

        if (!tableWin) {
            tableWin = new RestaurantTableWidget(nullptr);  // <-- پدر nullptr یعنی پنجره مستقل
            tableWin->setAttribute(Qt::WA_DeleteOnClose);
            connect(tableWin, &QObject::destroyed, this, [this]() { tableWin = nullptr; });

            tableWin->setWindowTitle("لیست رستوران‌ها");
        }

        tableWin->show();
        tableWin->raise();

        clientSocket->sendMessage("GET_ALL_RESTAURANTS\n");
    });

    // ⬇️ اتصال دکمۀ «لیست کاربران»
    connect(ui->pushButton_2, &QPushButton::clicked, this, [this]() {
        if (!userTableWin) {
            userTableWin = new UserTableWidget(nullptr);
            userTableWin->setAttribute(Qt::WA_DeleteOnClose);
            connect(userTableWin, &QObject::destroyed, this, [this]() {
                userTableWin = nullptr;
            });
            userTableWin->setWindowTitle("جدول کاربران");
        }

        userTableWin->show();
        userTableWin->raise();

        // اگر از سرور اطلاعات کاربران را می‌گیری:
        clientSocket->sendMessage("GET_ALL_USERS\n");
    });


}

void AdminMainPage::handleServerMessage(const QString &msg)
{
    /* --- حالت‌های خطا یا خالی --- */
    if (msg == "RESTAURANT_LIST_ALL:EMPTY") {
        QMessageBox::information(this, u"اطلاع"_qs,
                                 u"هیچ رستورانی ثبت نشده است."_qs);
        return;
    }
    if (msg == "RESTAURANT_LIST_ALL_FAIL") {
        QMessageBox::warning(this, u"خطا"_qs,
                             u"دریافت فهرست رستوران‌ها ناموفق بود."_qs);
        return;
    }

    /* --- حالت موفق --- */
    if (msg.startsWith("RESTAURANT_LIST_ALL:")) {

        QString data = msg.mid(QStringLiteral("RESTAURANT_LIST_ALL:").size()).trimmed();
        QStringList rows = data.split(';', Qt::SkipEmptyParts);

        if (!tableWin) return;
        QTableWidget* table = tableWin->findChild<QTableWidget*>("tableWidget");
        if (!table) return;

        table->clearContents();
        table->setRowCount(rows.size());

        for (int r = 0; r < rows.size(); ++r) {
            QStringList parts = rows[r].split('|');
            if (parts.size() != 4) continue;

            table->setItem(r, 0, new QTableWidgetItem(parts[0])); // نام رستوران
            table->setItem(r, 1, new QTableWidgetItem(parts[1])); // نام مالک
            table->setItem(r, 2, new QTableWidgetItem(parts[2])); // آدرس
            table->setItem(r, 3, new QTableWidgetItem(parts[3])); // وضعیت
        }
    }

    if (msg == "USER_LIST:EMPTY") {
        QMessageBox::information(this, u"اطلاع"_qs, u"کاربری ثبت نشده است."_qs);
        return;
    }
    if (msg == "USER_LIST_FAIL") {
        QMessageBox::warning(this, u"خطا"_qs, u"دریافت فهرست کاربران ناموفق بود."_qs);
        return;
    }

    if (msg.startsWith("USER_LIST:")) {

        QString data = msg.mid(QStringLiteral("USER_LIST:").size()).trimmed();
        QStringList rows = data.split(';', Qt::SkipEmptyParts);

        if (!userTableWin) return;
        QTableWidget *tbl = userTableWin->findChild<QTableWidget*>("tableWidget");
        if (!tbl) return;

        tbl->clearContents();
        tbl->setRowCount(rows.size());

        for (int r = 0; r < rows.size(); ++r) {
            QStringList p = rows[r].split('|');
            if (p.size() != 3) continue;

            tbl->setItem(r, 0, new QTableWidgetItem(p[0])); // نام
            tbl->setItem(r, 1, new QTableWidgetItem(p[1])); // تلفن
            tbl->setItem(r, 2, new QTableWidgetItem(p[2])); // نقش
            tbl->setItem(r, 3, new QTableWidgetItem(""));   // (اگر ستونی برای وضعیت می‌خواهی)
        }
    }

}


AdminMainPage::~AdminMainPage()
{
    delete ui;
}
