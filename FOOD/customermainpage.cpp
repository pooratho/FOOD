#include "customermainpage.h"
#include "ui_customermainpage.h"
#include <QDebug>
#include<QMessageBox>

CustomerMainPage::CustomerMainPage(Customer* customer, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CustomerMainPage)
    , customer(customer)
{
    ui->setupUi(this);

    // مقداردهی به clientSocket
    clientSocket = new ClientSocketManager(this);
    clientSocket->connectToServer("127.0.0.1", 1234);

    // اتصال سیگنال دریافت پیام
    connect(clientSocket, &ClientSocketManager::messageReceived,
            this, &CustomerMainPage::handleServerMessage);

    connect(clientSocket, &ClientSocketManager::connected, this, [=]() {
        qDebug() << "✅ اتصال به سرور برقرار شد";
        clientSocket->sendMessage("GET_RESTAURANTS");  // اینجا بفرست، نه بالا!
    });


    // نقشه استان‌ها
    provinceCitiesMap["آذربایجان شرقی"] = {"میانه", "اهر", "تبریز", "مرند", "مراغه"};
    provinceCitiesMap["اصفهان"] = {"اصفهان", "کاشان", "خمینی شهر", "شاهین شهر", "نجف آباد"};
    provinceCitiesMap["البرز"] = {"کرج", "فردیس", "کمال شهر", "نظر آباد", "محمد شهر"};
    provinceCitiesMap["تهران"] = {"تهران", "اسلام شهر", "شهریار", "قدس", "ملارد"};
    provinceCitiesMap["خراسان رضوی"] = {"مشهد", "نیشابور", "سبزوار", "تربت حیدریه", "کاشمر"};
    provinceCitiesMap["خوزستان"] = {"اهواز", "دزفول", "آبادان", "ماهشهر", "اندیمشک"};
    provinceCitiesMap["فارس"] = {"شیراز", "مرودشت", "جهرم", "فسا", "کازرون"};
    provinceCitiesMap["کردستان"] = {"سنندج", "سقز", "مریوان", "بانه", "قروه"};
    provinceCitiesMap["گیلان"] = {"رشت", "بندر انزلی", "لاهیجان", "لنگرود", "تالش"};
    provinceCitiesMap["مازندران"] = {"ساری", "بابل", "آمل", "قائم شهر", "بهشهر"};

    QStringList provinces = provinceCitiesMap.keys();
    provinces.sort();
    ui->comboBox_2->addItems(provinces);

    if (!provinces.isEmpty())
        on_comboBox_2_currentTextChanged(provinces.first());
}

CustomerMainPage::~CustomerMainPage()
{
    delete ui;
}

void CustomerMainPage::on_comboBox_2_currentTextChanged(const QString &province)
{
    qDebug() << "استان انتخاب شده: " << province;

    QStringList cities = provinceCitiesMap.value(province);
    cities.sort();

    ui->comboBox_3->clear();
    ui->comboBox_3->addItems(cities);
}

void CustomerMainPage::on_comboBox_3_currentTextChanged(const QString &city)
{
    qDebug() << "شهر انتخاب شده: " << city;
}

void CustomerMainPage::handleServerMessage(const QString& msg)
{
     QMessageBox::information(this, "رسیدن پیام", msg);  // تست اولیه
     qDebug() << "پیام از سرور: " << msg;
    if (msg.startsWith("RESTAURANT_LIST:")) {
        QString data = msg.mid(QString("RESTAURANT_LIST:").length());
        QStringList rows = data.split(";", Qt::SkipEmptyParts);

        ui->tableWidget->setRowCount(rows.size());
        ui->tableWidget->setColumnCount(3);
        ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "نام رستوران" << "نوع غذا" << "آدرس");

        for (int i = 0; i < rows.size(); ++i) {
            QStringList columns = rows[i].split("|");
            if (columns.size() != 3)
                continue;

            for (int j = 0; j < 3; ++j) {
                QTableWidgetItem *item = new QTableWidgetItem(columns[j]);
                item->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(i, j, item);
            }
        }

    }
    else if (msg.trimmed() == "RESTAURANT_LIST:EMPTY") {
        QMessageBox::information(this, "اطلاع", "هیچ رستورانی در سیستم ثبت نشده است.");
    }


    else if (msg.startsWith("RESTAURANT_LIST_FAIL")) {
        QMessageBox::warning(this, "خطا", "دریافت لیست رستوران‌ها ناموفق بود!");
    }
}


