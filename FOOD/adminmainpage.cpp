#include "adminmainpage.h"
#include "ui_adminmainpage.h"
#include "restauranttablewidget.h"
#include "userstablewidget.h"

#include <QTableWidget>
#include <QMessageBox>
#include <QFileDialog>


AdminMainPage::AdminMainPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AdminMainPage)
    , clientSocket(new ClientSocketManager(this))
{
    ui->setupUi(this);
    setWindowTitle(u"پنل مدیریت سیستم"_qs);

    connect(clientSocket, &ClientSocketManager::messageReceived,
            this, &AdminMainPage::handleServerMessage);

    clientSocket->connectToServer("127.0.0.1", 1234);

    connect(ui->pushButton_2, &QPushButton::clicked, this, [this]() {
        if (!userTableWin) {
            userTableWin = new UserTableWidget(clientSocket);
            userTableWin->setAttribute(Qt::WA_DeleteOnClose);
            connect(userTableWin, &QObject::destroyed, this, [this]() {
                userTableWin = nullptr;
            });
            userTableWin->setWindowTitle("جدول کاربران");
        }
        userTableWin->show();
        userTableWin->raise();
        clientSocket->sendMessage("GET_ALL_USERS\n");
    });

    connect(ui->pushButton, &QPushButton::clicked, this, [this]() {
        if (!tableWin) {

            tableWin = new RestaurantTableWidget(clientSocket);
            tableWin->setAttribute(Qt::WA_DeleteOnClose);
            connect(tableWin, &QObject::destroyed, this, [this]() { tableWin = nullptr; });
            tableWin->setWindowTitle("لیست رستوران‌ها");
        }
        tableWin->show();
        tableWin->raise();
        clientSocket->sendMessage("GET_ALL_RESTAURANTS\n");
    });

    connect(ui->pushButton_3, &QPushButton::clicked, this, [this]() {
        if (!ordersTableWin) {
            ordersTableWin = new OrdersTableWidget(nullptr);
            ordersTableWin->setAttribute(Qt::WA_DeleteOnClose);
            connect(ordersTableWin, &QObject::destroyed, this, [this]() {
                ordersTableWin = nullptr;
            });
            ordersTableWin->setWindowTitle("جدول سفارشات");
        }
        ordersTableWin->show();
        ordersTableWin->raise();
        clientSocket->sendMessage("GET_ALL_ORDERS\n");
    });

    connect(ui->pushButton_4, &QPushButton::clicked, this, [this]() {

        QString folder = QFileDialog::getExistingDirectory(
            this, QStringLiteral("انتخاب محل ذخیره‌سازی"));
        if (folder.isEmpty()) return;               // کاربر منصرف شد
        exportDestination = folder;                 // متغیر عضو کلاس

        clientSocket->sendMessage("EXPORT_DB\n");   // یا EXPORT_XLS
    });

}

static QList<QStringList> tempOrderRows;
void AdminMainPage::handleServerMessage(const QString &msg)
{
    qDebug() << "Message from server:" << msg;

    if (msg == "RESTAURANT_LIST_ALL:EMPTY") {
        QMessageBox::information(this, u"اطلاع"_qs, u"هیچ رستورانی ثبت نشده است."_qs);
        return;
    }
    if (msg == "RESTAURANT_LIST_ALL_FAIL") {
        QMessageBox::warning(this, u"خطا"_qs, u"دریافت فهرست رستوران‌ها ناموفق بود."_qs);
        return;
    }

    if (msg.startsWith("RESTAURANT_LIST_ALL:")) {
        QString data = msg.mid(QStringLiteral("RESTAURANT_LIST_ALL:").size()).trimmed();
        QStringList rows = data.split(';', Qt::SkipEmptyParts);

        if (!tableWin) return;
        QTableWidget* table = tableWin->findChild<QTableWidget*>("tableWidget");
        if (!table) return;

        table->clearContents();
        table->setRowCount(rows.size());

        // چون حالا 5 ستون داریم
        table->setColumnCount(5);
        table->setHorizontalHeaderLabels({"ID", "نام رستوران", "نام مالک", "آدرس", "وضعیت"});

        for (int r = 0; r < rows.size(); ++r) {
            QStringList parts = rows[r].split('|');
            if (parts.size() != 5) continue;  // الان انتظار 5 فیلد هست

            // مقداردهی سلول‌ها
            table->setItem(r, 0, new QTableWidgetItem(parts[0])); // id
            table->setItem(r, 1, new QTableWidgetItem(parts[1])); // نام رستوران
            table->setItem(r, 2, new QTableWidgetItem(parts[2])); // نام مالک
            table->setItem(r, 3, new QTableWidgetItem(parts[3])); // آدرس
            table->setItem(r, 4, new QTableWidgetItem(parts[4])); // وضعیت
        }
    }


    if (msg == "USER_LIST:EMPTY") {
        QMessageBox::information(this, u"اطلاع"_qs,
                                 u"کاربری ثبت نشده است."_qs);
        return;
    }

    if (msg == "USER_LIST_FAIL") {
        QMessageBox::warning(this, u"خطا"_qs,
                             u"دریافت فهرست کاربران ناموفق بود."_qs);
        return;
    }

    if (msg.startsWith("USER_LIST:")) {

        QString data = msg.mid(QStringLiteral("USER_LIST:").size()).trimmed();
        QStringList rows = data.split(';', Qt::SkipEmptyParts);

        if (!userTableWin) return;
        QTableWidget* tbl = userTableWin->findChild<QTableWidget*>("tableWidget");
        if (!tbl) return;

        tbl->clearContents();
        tbl->setColumnCount(4);                          // ➊ چهار ستون
        tbl->setHorizontalHeaderLabels(
            {u"نام"_qs, u"تلفن"_qs, u"نقش"_qs, u"وضعیت"_qs});

        tbl->setRowCount(rows.size());

        for (int r = 0; r < rows.size(); ++r) {
            QStringList p = rows[r].split('|');
            if (p.size() != 4) continue;                 // ➋ حالا باید 4 بخش باشد


            tbl->setItem(r, 0, new QTableWidgetItem(p[0])); // نام
            tbl->setItem(r, 1, new QTableWidgetItem(p[1])); // تلفن
            tbl->setItem(r, 2, new QTableWidgetItem(p[2])); // نقش
            tbl->setItem(r, 3, new QTableWidgetItem(p[3])); // وضعیت (Active/Blocked)
        }
    }



    if (msg.startsWith("ALL_ORDER:")) {
        QString data = msg.mid(QString("ALL_ORDER:").length()).trimmed();

        // مثلا: 0912...#12|52000|ثبت شده|2025-07-03 12:00:00|رستورانX,کباب,2,25000|رستورانX,نوشابه,1,2000
        QStringList sections = data.split('|', Qt::SkipEmptyParts);
        if (sections.size() < 4) return;

        QStringList idAndPhone = sections[0].split('#');
        if (idAndPhone.size() < 3) return;

        QString phone = idAndPhone[0];
        QString customerName = idAndPhone[1];

        QString orderId = idAndPhone[1];
        QString totalPrice = sections[1];
        QString status = sections[2];

        // شروع از index 3 به بعد آیتم‌ها هستن
        for (int i = 3; i < sections.size(); ++i) {
            QStringList itemParts = sections[i].split(',');
            if (itemParts.size() != 4) continue;

            QString restaurantName = itemParts[0];
            QString foodName = itemParts[1];
            QString quantity = itemParts[3]; // چون ترتیب: name, food, price, quantity
            QString unitPrice = itemParts[2];

            QStringList row;
            row << restaurantName << foodName << unitPrice << quantity << customerName << status;
            tempOrderRows.append(row);
        }
    }
    else if (msg == "ALL_ORDERS_DONE") {
        if (!ordersTableWin) return;

        QTableWidget *tbl = ordersTableWin->findChild<QTableWidget*>("tableWidget");
        if (!tbl) return;

        tbl->clearContents();
        tbl->setRowCount(tempOrderRows.size());

        for (int r = 0; r < tempOrderRows.size(); ++r) {
            const QStringList& row = tempOrderRows[r];
            for (int c = 0; c < row.size(); ++c) {
                tbl->setItem(r, c, new QTableWidgetItem(row[c]));
            }
        }

        tempOrderRows.clear();
    }
    else if (msg.startsWith("ALL_ORDERS_EMPTY:")) {
        QMessageBox::information(this, u"اطلاع"_qs, msg.section(':', 1));
    }

}

AdminMainPage::~AdminMainPage()
{
    delete ui;
}
