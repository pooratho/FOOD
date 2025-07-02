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

    QString restaurantName = currentOwner->getRestaurant().getName();
    ui->label_10->setText("رستوران:  " + restaurantName);

    connect(clientSocket, &ClientSocketManager::messageReceived,
            this, &RestaurantOwnerMainPage::handleServerMessage);

    connect(ui->tabWidget, &QTabWidget::currentChanged,
            this, &RestaurantOwnerMainPage::on_tabWidget_currentChanged);


    connect(clientSocket, &ClientSocketManager::connected, this, [=]() {
        qDebug() << "Connected to server, sending REGISTER_RESTAURANT_SOCKET";
        qDebug() << "Registering restaurant name:" << currentOwner->getRestaurant().getName();

        // ارسال پیام ثبت نام رستوران به سرور
        clientSocket->sendMessage("REGISTER_RESTAURANT_SOCKET:" + currentOwner->getRestaurant().getName());
        // حذف ارسال GET_MENU از اینجا، بعد از دریافت REGISTER_OK ارسال می‌کنیم
    });

    clientSocket->connectToServer("127.0.0.1", 1234);

    ui->tabWidget->setCurrentIndex(1);
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
        qDebug() << "handleServerMessage called with msg:" << msg;
    if (msg.startsWith("REGISTER_OK")) {
        clientSocket->sendMessage("GET_MENU");
    }
    else if (msg.startsWith("MENU_LIST:")) {
        QString rawItems = msg.mid(QString("MENU_LIST:").length());

        if (rawItems.trimmed().isEmpty()) {
            qDebug() << "Empty menu received, skipping populate.";
            return;  // منو خالیه، پس چیزی نشون نده
        }

        QStringList items = rawItems.split(";", Qt::SkipEmptyParts);
        populateMenuItems(items);
    }

    else if (msg.startsWith("ADD_FOOD_OK")) {
        QMessageBox::information(this, "موفق", "غذا با موفقیت ذخیره شد.");
        clientSocket->sendMessage("GET_MENU");
    }
    else if (msg.startsWith("ADD_FOOD_FAIL:")) {
        QString reason = msg.section(':', 1);
        QMessageBox::warning(this, "خطا در افزودن غذا", reason);
    }
    else if (msg.startsWith("DELETE_FOOD_OK")) {
        if (!pendingDeletions.isEmpty()) {
            auto [listWidget, itemWidget] = pendingDeletions.takeLast();

            for (int i = 0; i < listWidget->count(); ++i) {
                QListWidgetItem* listItem = listWidget->item(i);
                if (listWidget->itemWidget(listItem) == itemWidget) {
                    listWidget->removeItemWidget(listItem); // قطع ارتباط ویجت با آیتم
                    delete listItem;                         // حذف آیتم لیست
                    itemWidget->deleteLater();               // حذف امن ویجت
                    break;
                }
            }
        }

        QMessageBox::information(this, "موفق", "غذا با موفقیت حذف شد.");
      //  clientSocket->sendMessage("GET_MENU");
    }
    else if (msg.startsWith("DELETE_FOOD_FAIL:")) {
        QString reason = msg.section(':', 1);
        QMessageBox::warning(this, "خطا", reason);
    }
    else if (msg.startsWith("RESTAURANT_ORDER:")) {
        QString data = msg.mid(QString("RESTAURANT_ORDER:").length()).trimmed();
        QStringList parts = data.split("#");
        if (parts.size() != 2) return;

        QString customerPhone = parts[0];
        QStringList orderParts = parts[1].split("|");
        if (orderParts.size() < 4) return;

        QString orderId = orderParts[0];
        double totalPrice = orderParts[1].toDouble();
        QString status = orderParts[2];
        QString createdAt = orderParts[3];

        QString foodDetails;
        for (int i = 3; i < orderParts.size(); ++i) {
                    qDebug() << "[RAW FOOD PART]" << orderParts[i];
            QStringList food = orderParts[i].split(",");
                            qDebug() << "split size =" << food.size() << "parts =" << food;
            if (food.size() != 3) {
                                            qWarning() << "⚠️  آیتم رد شد (size != 3)";
                continue;
            }

            foodDetails += "  " + food[0] + " × " + food[1] + " - " + food[2] + " تومان\n";
        }

        // ساخت ویجت سفارش رستوران
        RestaurantOwnerOrderItemWidget* widget = new RestaurantOwnerOrderItemWidget(this);

        // مقداردهی فیلدها
        widget->setCustomerPhone(customerPhone);
        widget->setFoodText(foodDetails);
        widget->setStatus(status);


        QListWidgetItem* item = new QListWidgetItem(ui->orderListWidget);
        item->setSizeHint(QSize(600, 130));
        ui->orderListWidget->addItem(item);
        ui->orderListWidget->setItemWidget(item, widget);

        // اتصال سیگنال تغییر وضعیت به ارسال پیام به سرور
        connect(widget, &RestaurantOwnerOrderItemWidget::updateStatusRequested, this, [=](const QString& newStatus) {
            QString updateMsg = "UPDATE_ORDER_STATUS:" + orderId + "#" + newStatus;
            clientSocket->sendMessage(updateMsg);
        });
    }

    // به‌روزرسانی نمایش برچسب‌ها
    ui->label_5->setVisible(ui->listWidgetMain->count() == 0);
    ui->label_9->setVisible(ui->listWidgetDessert->count() == 0);
    ui->label_8->setVisible(ui->listWidgetDrink->count() == 0);
    ui->label_7->setVisible(ui->listWidgetStarter->count() == 0);
    ui->label_6->setVisible(ui->listWidgetOthers->count() == 0);

    ui->label_11->setVisible(ui->orderListWidget->count() == 0);
}

void RestaurantOwnerMainPage::populateMenuItems(const QStringList& items)
{
      qDebug() << "populateMenuItems called with" << items.size() << "items";
    menuByCategory.clear();

    clearListWidgetCompletely(ui->listWidgetMain);
    clearListWidgetCompletely(ui->listWidgetDessert);
    clearListWidgetCompletely(ui->listWidgetDrink);
    clearListWidgetCompletely(ui->listWidgetStarter);
    clearListWidgetCompletely(ui->listWidgetOthers);

    for (const QString& item : items) {
        QStringList parts = item.split("|");
        if (parts.size() != 4) continue;

        QString name = parts[0];
        QString desc = parts[1];
        double price = parts[2].toDouble();
        QString cat = parts[3];

        Food food(name, desc, price, cat);
        menuByCategory[cat].addFood(food);

        RestaurantOwnerMenuItemWidget* itemWidget = new RestaurantOwnerMenuItemWidget(this);
        itemWidget->setFood(food);
        itemWidget->setItemInfo(food);

        QListWidgetItem* listItem = new QListWidgetItem();
        listItem->setSizeHint(QSize(610, 102));

        QListWidget* targetListWidget = nullptr;
        if (cat == "دسر")        targetListWidget = ui->listWidgetDessert;
        else if (cat == "نوشیدنی") targetListWidget = ui->listWidgetDrink;
        else if (cat == "پیش غذا") targetListWidget = ui->listWidgetStarter;
        else if (cat == "غذا")     targetListWidget = ui->listWidgetMain;
        else                      targetListWidget = ui->listWidgetOthers;

        targetListWidget->addItem(listItem);
        targetListWidget->setItemWidget(listItem, itemWidget);

        connect(itemWidget, &RestaurantOwnerMenuItemWidget::removeRequested, this, [=]() {
            disconnect(itemWidget, nullptr, nullptr, nullptr);

             QMessageBox::StandardButton reply = QMessageBox::question(this, "تأیید حذف",
                                                                       "آیا مطمئن هستید که می‌خواهید این آیتم را حذف کنید؟",
                                                                       QMessageBox::Yes | QMessageBox::No);
             if (reply == QMessageBox::Yes) {
                QString foodName = itemWidget->getFood().getName();
                QString category = itemWidget->getFood().getCategory();
                QString deleteMsg = "DELETE_FOOD:" + category + ":" + foodName;

                // ثبت حذف در pendingDeletions برای حذف امن بعد از پاسخ سرور
                pendingDeletions.append(qMakePair(targetListWidget, itemWidget));

                clientSocket->sendMessage(deleteMsg);
           }
        });
    }

    ui->label_5->setVisible(ui->listWidgetMain->count() == 0);
    ui->label_9->setVisible(ui->listWidgetDessert->count() == 0);
    ui->label_8->setVisible(ui->listWidgetDrink->count() == 0);
    ui->label_7->setVisible(ui->listWidgetStarter->count() == 0);
    ui->label_6->setVisible(ui->listWidgetOthers->count() == 0);
}

void RestaurantOwnerMainPage::clearListWidgetCompletely(QListWidget* listWidget)
{
    while (listWidget->count() > 0) {
        QListWidgetItem* item = listWidget->takeItem(0);
        if (item) {
            QWidget* w = listWidget->itemWidget(item);
            if (w) w->deleteLater();  // حذف امن ویجت
            delete item;              // حذف آیتم لیست
        }
    }
}

void RestaurantOwnerMainPage::on_tabWidget_currentChanged(int index)
{
    // فرض: تب دوم مربوط به سفارش‌هاست و index = 1
    if (index == 2) {
        QString msg = "GET_RESTAURANT_ORDERS:" + currentOwner->getRestaurant().getName();

        clientSocket->sendMessage(msg);

        // قبلش لیست سفارشات قبلی رو پاک کن:
        ui->orderListWidget->clear();
    }
}
