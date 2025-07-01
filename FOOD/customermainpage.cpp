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

    connect(ui->comboBoxProvince, &QComboBox::currentTextChanged,
            this, &CustomerMainPage::sendFilteredRequest);

    connect(ui->comboBoxCity, &QComboBox::currentTextChanged,
            this, &CustomerMainPage::sendFilteredRequest);

    connect(ui->comboBoxFoodType, &QComboBox::currentTextChanged,
            this, &CustomerMainPage::sendFilteredRequest);

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
    ui->comboBoxProvince->addItems(provinces);

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

    ui->comboBoxCity->clear();
    ui->comboBoxCity->addItems(cities);
}

void CustomerMainPage::on_comboBox_3_currentTextChanged(const QString &city)
{
    qDebug() << "شهر انتخاب شده: " << city;
}

void CustomerMainPage::handleServerMessage(const QString& msg)
{
    qDebug() << "پیام از سرور: " << msg;

    if (msg.startsWith("RESTAURANT_LIST:")) {
        QString data = msg.mid(QString("RESTAURANT_LIST:").length());
        QStringList rows = data.split(";", Qt::SkipEmptyParts);

        // جدول رو پاک می‌کنیم
        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);

        // فقط یک بار ستون‌ها و هدر رو تنظیم کن (اگر قبلاً انجام نشده)
        if (ui->tableWidget->columnCount() != 3) {
            ui->tableWidget->setColumnCount(3);
            ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "نام رستوران" << "نوع غذا" << "آدرس");
        }

        ui->tableWidget->setRowCount(rows.size());

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

        // اگر هیچ رستوانی نبود
        if (rows.isEmpty()) {
            QMessageBox::information(this, "اطلاع", "هیچ رستورانی در سیستم ثبت نشده است.");
        }
    }
    else if (msg.startsWith("RESTAURANT_LIST_FAIL")) {
        QMessageBox::warning(this, "خطا", "دریافت لیست رستوران‌ها ناموفق بود!");
    }
    else {
        qDebug() << "پیام ناشناخته از سرور دریافت شد.";
    }
}


void CustomerMainPage::sendFilteredRequest()
{
    QString province = ui->comboBoxProvince->currentText().trimmed();
    QString city = ui->comboBoxCity->currentText().trimmed();
    QString type = ui->comboBoxFoodType->currentText().trimmed();

    if (province.isEmpty() && city.isEmpty() && type.isEmpty()) {
        clientSocket->sendMessage("GET_RESTAURANTS");
        return;
    }

    QStringList filters;
    if (!province.isEmpty())
        filters << "province=" + province;
    if (!city.isEmpty())
        filters << "city=" + city;
    if (!type.isEmpty())
        filters << "type=" + type;

    QString msg = "GET_RESTAURANTS_FILTERED:" + filters.join(";");

    clientSocket->sendMessage(msg);
}


