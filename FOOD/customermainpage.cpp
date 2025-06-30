#include "customermainpage.h"
#include "ui_customermainpage.h"
#include <QDebug>

CustomerMainPage::CustomerMainPage(Customer* customer, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CustomerMainPage)
    , customer(customer)
{
    ui->setupUi(this);



    // تعریف نقشه استان -> لیست شهرها
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

    // اضافه کردن استان‌ها
    QStringList provinces = provinceCitiesMap.keys();
    provinces.sort();
    ui->comboBox_2->addItems(provinces);

    // انتخاب پیش‌فرض (مثلاً اولین استان)
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
