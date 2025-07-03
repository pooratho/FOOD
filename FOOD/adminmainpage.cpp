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

        clientSocket->sendMessage("GET_RESTAURANTS\n");
    });

}

void AdminMainPage::handleServerMessage(const QString &msg)
{
    /* --- حالت‌های خطا یا خالی --- */
    if (msg == "RESTAURANT_LIST:EMPTY") {
        QMessageBox::information(this, u"اطلاع"_qs,
                                 u"هیچ رستورانی ثبت نشده است."_qs);
        return;
    }
    if (msg == "RESTAURANT_LIST_FAIL") {
        QMessageBox::warning(this, u"خطا"_qs,
                             u"دریافت فهرست رستوران‌ها ناموفق بود."_qs);
        return;
    }

    /* --- حالت موفق --- */
    if (msg.startsWith("RESTAURANT_LIST:")) {

        QString data = msg.mid(QStringLiteral("RESTAURANT_LIST:").size()).trimmed();
        QStringList rows = data.split(';', Qt::SkipEmptyParts);

        /* از پوینتر عضو که در سازنده ساختیم استفاده می‌کنیم */
        if (!tableWin) return;                             // شاید کاربر پنجره را بسته
        QTableWidget* table = tableWin->findChild<QTableWidget*>("tableWidget");
        if (!table) return;

        table->clearContents();            // پاک کردن قبلی
        table->setRowCount(rows.size());   // تعداد ردیف‌ها

        for (int r = 0; r < rows.size(); ++r) {
            QStringList parts = rows[r].split('|');
            if (parts.size() != 3) continue;   // دادهٔ خراب

            table->setItem(r, 0, new QTableWidgetItem(QString::number(r + 1))); // ردیف
            table->setItem(r, 1, new QTableWidgetItem(parts[0]));               // نام رستوران
            table->setItem(r, 2, new QTableWidgetItem(parts[1]));               // نام مالک یا نوع
            table->setItem(r, 3, new QTableWidgetItem(parts[2]));               // وضعیت / آدرس
        }
    }
}


AdminMainPage::~AdminMainPage()
{
    delete ui;
}
