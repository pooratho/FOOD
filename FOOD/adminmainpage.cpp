#include "adminmainpage.h"
#include "ui_adminmainpage.h"
#include "restauranttablewidget.h"

#include <QTableWidget>
#include <QMessageBox>

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

    connect(ui->pushButton, &QPushButton::clicked, this, [this]() {
        if (!tableWin) {
            tableWin = new RestaurantTableWidget(nullptr);
            tableWin->setAttribute(Qt::WA_DeleteOnClose);
            connect(tableWin, &QObject::destroyed, this, [this]() { tableWin = nullptr; });
            tableWin->setWindowTitle("لیست رستوران‌ها");
        }
        tableWin->show();
        tableWin->raise();
        clientSocket->sendMessage("GET_ALL_RESTAURANTS\n");
    });

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
        clientSocket->sendMessage("GET_ALL_USERS\n");
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
}

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

        for (int r = 0; r < rows.size(); ++r) {
            QStringList parts = rows[r].split('|');
            if (parts.size() != 4) continue;

            table->setItem(r, 0, new QTableWidgetItem(parts[0]));
            table->setItem(r, 1, new QTableWidgetItem(parts[1]));
            table->setItem(r, 2, new QTableWidgetItem(parts[2]));
            table->setItem(r, 3, new QTableWidgetItem(parts[3]));
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

        userPhoneToName.clear();

        if (!userTableWin) return;
        QTableWidget *tbl = userTableWin->findChild<QTableWidget*>("tableWidget");
        if (!tbl) return;

        tbl->clearContents();
        tbl->setRowCount(rows.size());

        for (int r = 0; r < rows.size(); ++r) {
            QStringList p = rows[r].split('|');
            if (p.size() != 3) continue;

            userPhoneToName[p[1]] = p[0];

            tbl->setItem(r, 0, new QTableWidgetItem(p[0]));
            tbl->setItem(r, 1, new QTableWidgetItem(p[1]));
            tbl->setItem(r, 2, new QTableWidgetItem(p[2]));
            tbl->setItem(r, 3, new QTableWidgetItem(""));
        }
    }

    if (msg == "ORDER_LIST:EMPTY") {
        QMessageBox::information(this, u"اطلاع"_qs, u"هیچ سفارشی ثبت نشده است."_qs);
        return;
    }

    if (msg == "ORDER_LIST_FAIL") {
        QMessageBox::warning(this, u"خطا"_qs, u"دریافت فهرست سفارش‌ها ناموفق بود."_qs);
        return;
    }

    if (msg.startsWith("ORDER_LIST:")) {
        QString data = msg.mid(QStringLiteral("ORDER_LIST:").size()).trimmed();
        QStringList rows = data.split(';', Qt::SkipEmptyParts);

        if (!ordersTableWin) return;
        QTableWidget *tbl = ordersTableWin->findChild<QTableWidget*>("tableWidget");
        if (!tbl) return;

        tbl->clearContents();
        tbl->setRowCount(rows.size());

        for (int r = 0; r < rows.size(); ++r) {
            QStringList parts = rows[r].split('|');
            if (parts.size() < 6) continue;

            QString phone = parts[1];
            QString customerName = userPhoneToName.value(phone, u"نامشخص"_qs);

            tbl->setItem(r, 0, new QTableWidgetItem(customerName));
            tbl->setItem(r, 1, new QTableWidgetItem(phone));
            tbl->setItem(r, 2, new QTableWidgetItem(parts[2]));
            tbl->setItem(r, 3, new QTableWidgetItem(parts[3]));
            tbl->setItem(r, 4, new QTableWidgetItem(parts[4]));
            tbl->setItem(r, 5, new QTableWidgetItem(parts[5]));
        }
    }
}

AdminMainPage::~AdminMainPage()
{
    delete ui;
}
