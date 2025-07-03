#include "adminmainpage.h"
#include "ui_adminmainpage.h"
#include "restauranttablewidget.h"

#include <QTableWidget>

// ✅ هدر جدول رستوران‌ها

AdminMainPage::AdminMainPage(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::AdminMainPage)
{
    ui->setupUi(this);
    setWindowTitle(u"پنل مدیریت سیستم"_qs);

    // ⬇️ اتصال دکمۀ اول به باز کردن جدول رستوران‌ها
    connect(ui->pushButton, &QPushButton::clicked, this, [this]() {
        auto *tableWin = new RestaurantTableWidget;
        tableWin->setAttribute(Qt::WA_DeleteOnClose);
        tableWin->setWindowModality(Qt::ApplicationModal);
        tableWin->show();

        clientSocket->sendMessage("GET_RESTAURANT_TABLE");  // 📤 درخواست به سرور
    });

    clientSocket = new ClientSocketManager(this);
    clientSocket->connectToServer("127.0.0.1", 1234);  // آدرس سرور

    connect(clientSocket, &ClientSocketManager::messageReceived,
            this, &AdminMainPage::handleServerMessage);  // پیام‌های دریافتی

}

void AdminMainPage::handleServerMessage(const QString &msg)
{
    if (msg.startsWith("RESTAURANT_TABLE:")) {
        QString data = msg.mid(QString("RESTAURANT_TABLE:").length());
        QStringList rows = data.split(";", Qt::SkipEmptyParts);

        // فرض می‌کنیم فقط یک جدول باز شده (برای ساده‌سازی)
        RestaurantTableWidget* tableWin = findChild<RestaurantTableWidget*>();
        if (!tableWin) return;

        QTableWidget* table = tableWin->findChild<QTableWidget*>("tableWidget");
        if (!table) return;

        table->setRowCount(0);
        int row = 0;

        for (const QString& line : rows) {
            QStringList parts = line.split("|");
            if (parts.size() != 3) continue;

            table->insertRow(row);
            table->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));
            table->setItem(row, 1, new QTableWidgetItem(parts[0]));  // نام رستوران
            table->setItem(row, 2, new QTableWidgetItem(parts[1]));  // نام مالک
            table->setItem(row, 3, new QTableWidgetItem(parts[2]));  // وضعیت
            row++;
        }
    }
}

AdminMainPage::~AdminMainPage()
{
    delete ui;
}
