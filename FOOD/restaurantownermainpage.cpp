#include "restaurantownermainpage.h"
#include "ui_restaurantownermainpage.h"
#include <QMessageBox>

RestaurantOwnerMainPage::RestaurantOwnerMainPage(RestaurantOwner* owner, QWidget *parent)
    : QWidget(parent),
    ui(new Ui::restaurantOwnerMainPage),
    currentOwner(owner)
{
    ui->setupUi(this);

    connect(clientSocket, &ClientSocketManager::messageReceived,
            this, &RestaurantOwnerMainPage::handleServerMessage);

    // بعد از اتصال موفق:
    connect(clientSocket, &ClientSocketManager::connected, this, [=]() {
        clientSocket->sendMessage("GET_MENU");
    });


}

RestaurantOwnerMainPage::~RestaurantOwnerMainPage()
{
    delete ui;
}

void RestaurantOwnerMainPage::on_pushButton_clicked()
{
    QString category = ui->comboBoxCategory->currentText().trimmed();
    QString name = ui->lineEditFoodName->text().trimmed();
    QString priceStr = ui->lineEditPrice->text().trimmed();
    QString desc = ui->textEditDescription->text().trimmed();

    // بررسی مقادیر ورودی و ساخت منو
    if (name.isEmpty() || priceStr.isEmpty()) {
        QMessageBox::warning(this, "خطا", "لطفا نام غذا و قیمت را وارد کنید.");
        return;
    }

    bool ok;
    double price = priceStr.toDouble(&ok);
    if (!ok) {
        QMessageBox::warning(this, "خطا", "قیمت وارد شده معتبر نیست.");
        return;
    }

    Food food(name, desc, price, category);

    currentOwner->getRestaurant().getMenu()[category].addFood(food);



    QMessageBox::information(this, "موفقیت", "غذا به منو اضافه شد.");

    // پاک کردن فیلدها برای وارد کردن غذاهای جدید
    ui->lineEditFoodName->clear();
    ui->lineEditPrice->clear();
    ui->textEditDescription->clear();
    ui->comboBoxCategory->setCurrentIndex(0);


}



void RestaurantOwnerMainPage::handleServerMessage(const QString& msg)
{
    if (msg.startsWith("MENU_LIST:")) {
        QStringList items = msg.mid(QString("MENU_LIST:").length()).split(";", Qt::SkipEmptyParts);

        // پاک کردن منوی قبلی
        menuByCategory.clear();

        // پاک کردن لیست‌ویجت‌ها
        ui->listWidgetMain->clear();
        ui->listWidgetDessert->clear();
        ui->listWidgetDrink->clear();
        ui->listWidgetStarter->clear();
        ui->listWidgetOthers->clear();

        for (const QString& item : items) {
            QStringList parts = item.split("|");
            if (parts.size() != 4) continue;

            QString name = parts[0];
            QString desc = parts[1];
            double price = parts[2].toDouble();
            QString cat = parts[3];

            Food food(name, desc, price, cat);

            // اضافه کردن غذا به منوی دسته‌بندی شده
            menuByCategory[cat].addFood(food);

            QString display = name + " - " + QString::number(price) + "\n" + desc;

            if (cat == "دسر")
                ui->listWidgetDessert->addItem(display);
            else if (cat == "نوشیدنی")
                ui->listWidgetDrink->addItem(display);
            else if (cat == "پیش غذا")
                ui->listWidgetStarter->addItem(display);
            else if (cat == "غذا")
                ui->listWidgetMain->addItem(display);
            else
                ui->listWidgetOthers->addItem(display);
        }
    }
}
