#include "restaurantownermainpage.h"
#include "ui_restaurantownermainpage.h"
#include "restaurantownermenuitemwidget.h"

#include <QMessageBox>

RestaurantOwnerMainPage::RestaurantOwnerMainPage(RestaurantOwner* owner, QWidget *parent)
    : QWidget(parent),
    ui(new Ui::restaurantOwnerMainPage),
    currentOwner(owner),
    clientSocket(new ClientSocketManager(this))
{
    ui->setupUi(this);

    connect(clientSocket, &ClientSocketManager::messageReceived,
            this, &RestaurantOwnerMainPage::handleServerMessage);

    connect(clientSocket, &ClientSocketManager::connected, this, [=]() {
        qDebug() << "Connected to server, sending REGISTER_RESTAURANT_SOCKET";
        qDebug() << "Registering restaurant name:" << currentOwner->getRestaurant().getName();

        // ارسال پیام ثبت نام رستوران به سرور
        clientSocket->sendMessage("REGISTER_RESTAURANT_SOCKET:" + currentOwner->getRestaurant().getName());
        // حذف ارسال GET_MENU از اینجا، بعد از دریافت REGISTER_OK ارسال می‌کنیم
    });

    clientSocket->connectToServer("127.0.0.1", 1234);
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

    // فرستادن غذا به سرور برای ذخیره در دیتابیس
    QString msg = "ADD_FOOD:" + category + ":" + name + ":" + QString::number(price) + ":" + desc;
    clientSocket->sendMessage(msg);

    QMessageBox::information(this, "موفقیت", "غذا به سرور ارسال شد.");
    ui->lineEditFoodName->clear();
    ui->lineEditPrice->clear();
    ui->textEditDescription->clear();
    ui->comboBoxCategory->setCurrentIndex(0);
}



void RestaurantOwnerMainPage::handleServerMessage(const QString& msg)
{
    if (msg.startsWith("REGISTER_OK")) {
        // وقتی سرور تایید ثبت نام رستوران رو داد، منو رو بگیر
        clientSocket->sendMessage("GET_MENU");
    }

    if (msg.startsWith("MENU_LIST:")) {
        QStringList items = msg.mid(QString("MENU_LIST:").length()).split(";", Qt::SkipEmptyParts);

        menuByCategory.clear();
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
            menuByCategory[cat].addFood(food);

            // ساخت ویجت سفارشی
            RestaurantOwnerMenuItemWidget* itemWidget = new RestaurantOwnerMenuItemWidget(this);
            itemWidget->setItemInfo("  " + name, "  " + desc, QString::number(price));

            QListWidgetItem* listItem = new QListWidgetItem();
            listItem->setSizeHint(QSize(610, 102));  // تعیین سایز آیتم

            QListWidget* targetListWidget = nullptr;
            if (cat == "دسر") {
                targetListWidget = ui->listWidgetDessert;
            } else if (cat == "نوشیدنی") {
                targetListWidget = ui->listWidgetDrink;
            } else if (cat == "پیش غذا") {
                targetListWidget = ui->listWidgetStarter;
            } else if (cat == "غذا") {
                targetListWidget = ui->listWidgetMain;
            } else {
                targetListWidget = ui->listWidgetOthers;
            }

            targetListWidget->addItem(listItem);
            targetListWidget->setItemWidget(listItem, itemWidget);

            // *** اینجا اتصال سیگنال حذف را اضافه کن ***
            connect(itemWidget, &RestaurantOwnerMenuItemWidget::removeRequested, this, [=]() {
                QMessageBox::StandardButton reply = QMessageBox::question(this, "تأیید حذف",
                                                                          "آیا مطمئن هستید که می‌خواهید این آیتم را حذف کنید؟",
                                                                          QMessageBox::Yes | QMessageBox::No);
                if (reply == QMessageBox::Yes) {
                    for (int i = 0; i < targetListWidget->count(); ++i) {
                        QListWidgetItem* listItemIter = targetListWidget->item(i);
                        QWidget* w = targetListWidget->itemWidget(listItemIter);
                        if (w == itemWidget) {
                            delete targetListWidget->takeItem(i);  // حذف آیتم و ویجت مرتبط
                            break;
                        }
                    }
                }
            });

        }
    }

    // ✅ واکنش به پاسخ سرور برای اضافه شدن غذا
    else if (msg.startsWith("ADD_FOOD_OK")) {
        QMessageBox::information(this, "✅ موفق", "غذا با موفقیت ذخیره شد.");
        clientSocket->sendMessage("GET_MENU");  // درخواست منوی جدید از سرور
    }

    else if (msg.startsWith("ADD_FOOD_FAIL:")) {
        QString reason = msg.section(':', 1);
        QMessageBox::warning(this, "❌ خطا در افزودن غذا", reason);
    }

    ui->label_5->setVisible(ui->listWidgetMain->count() == 0);
    ui->label_9->setVisible(ui->listWidgetDessert->count() == 0);
    ui->label_8->setVisible(ui->listWidgetDrink->count() == 0);
    ui->label_7->setVisible(ui->listWidgetStarter->count() == 0);
    ui->label_6->setVisible(ui->listWidgetOthers->count() == 0);

}
