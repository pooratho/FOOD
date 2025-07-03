#include "restaurantownermainpage.h"
#include "qtimer.h"
#include "ui_restaurantownermainpage.h"
#include "restaurantownermenuitemwidget.h"

#include <QMessageBox>
#include <QQuickWidget>
#include <QQmlContext>
#include <QVBoxLayout>

RestaurantOwnerMainPage::RestaurantOwnerMainPage(RestaurantOwner* owner, QWidget *parent)
    : QWidget(parent),
    ui(new Ui::restaurantOwnerMainPage),
    currentOwner(owner),
    clientSocket(new ClientSocketManager(this))
{
    ui->setupUi(this);

    QString restaurantName = currentOwner->getRestaurant().getName();
    ui->label_10->setText("رستوران:  " + restaurantName);

    connect(ui->tabWidget, &QTabWidget::currentChanged,
            this, &RestaurantOwnerMainPage::on_tabWidget_currentChanged);
    connect(clientSocket, &ClientSocketManager::messageReceived,
            this, &RestaurantOwnerMainPage::handleServerMessage);

    connect(clientSocket, &ClientSocketManager::connected, this, [=]() {
        qDebug() << "Connected to server";
        // فقط لاگین رو ارسال کن، نه ثبت سوکت
        QString loginMsg = QString("LOGIN:Restaurant:%1:%2:%3")
                               .arg(currentOwner->getFirstName())
                               .arg(currentOwner->getLastName())
                               .arg(currentOwner->getPassword());
        clientSocket->sendMessage(loginMsg);
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
    if (msg.startsWith("LOGIN_OK")) {
        qDebug() << "Login successful, registering restaurant socket...";
        registerRestaurantSocket();
        clientSocket->sendMessage("GET_MENU");
    }
    else if (msg.startsWith("REGISTER_OK")) {

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

        QString customerPhone = parts[0];   // شماره مشتری
        QString orderDataStr = parts[1];   // رشته شامل orderId و بقیه اطلاعات

        QStringList orderParts = orderDataStr.split("|");
        if (orderParts.size() < 4) return;

        QString orderId = orderParts[0];          // اینجا orderId دقیق استخراج میشه
        double totalPrice = orderParts[1].toDouble();
        QString status = orderParts[2];
        QString createdAt = orderParts[3];

        int norderId = orderId.toInt();

        if (shownOrderIds.contains(norderId)) {
            qDebug() << "❗ سفارش تکراری نادیده گرفته شد:" << norderId;
            return;
        }
        shownOrderIds.insert(norderId);



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

       // ثبت سفارش جدید
        // ... بعد از استخراج status
        if (status == u"تحویل داده شده") {
            return;                  // 👈 نمایش داده نشود
        }

        // ساخت ویجت سفارش رستوران
        RestaurantOwnerOrderItemWidget* widget = new RestaurantOwnerOrderItemWidget(this);

        widget->setCustomerPhone(customerPhone);
        widget->setFoodText(foodDetails);
        widget->setStatus(status);

        QListWidgetItem* item = new QListWidgetItem(ui->orderListWidget);
        item->setSizeHint(QSize(600, 130));
        ui->orderListWidget->addItem(item);
        ui->orderListWidget->setItemWidget(item, widget);

        connect(widget, &RestaurantOwnerOrderItemWidget::updateStatusRequested, this, [=](const QString& newStatus) {
            qDebug() << "Sending update status for orderId:" << orderId << "newStatus:" << newStatus;
            QString updateMsg = "UPDATE_ORDER_STATUS:" + orderId + "#" + newStatus;
            clientSocket->sendMessage(updateMsg);

            if (newStatus == u"تحویل داده شده") {
                for (int i = 0; i < ui->orderListWidget->count(); ++i) {
                    QListWidgetItem* listItem = ui->orderListWidget->item(i);
                    if (ui->orderListWidget->itemWidget(listItem) == widget) {
                        ui->orderListWidget->removeItemWidget(listItem);
                        delete listItem;
                        widget->deleteLater();
                        break;
                    }
                }
                ui->label_11->setVisible(ui->orderListWidget->count() == 0);
            }
        });


    }
    else if (msg.startsWith("NEW_ORDER_ALERT")) {
        qDebug() << "New order alert received, showing notification.";
        showNewOrderNotification("📦 سفارش جدیدی ثبت شد!");
    }
    else {
        qDebug() << "پیام ناشناخته از سرور دریافت شد:" << msg;
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
        clearOrderListWidget(); // پاک‌کردن آیتم‌ها
        shownOrderIds.clear();         // ❗ پاک‌کردن سفارش‌هایی که قبلاً نشون داده بودیم
        QString msg = "GET_RESTAURANT_ORDERS:" + currentOwner->getRestaurant().getName();
        clientSocket->sendMessage(msg);
        //notificationLabel->hide();
    }
}

void RestaurantOwnerMainPage::on_pushButton_2_clicked()
{

}
void RestaurantOwnerMainPage::clearOrderListWidget()
{
    while (ui->orderListWidget->count() > 0) {
        QListWidgetItem* item = ui->orderListWidget->takeItem(0);
        if (item) {
            QWidget* w = ui->orderListWidget->itemWidget(item);
            if (w) w->deleteLater();  // حذف امن ویجت
            delete item;
        }
    }
}
void RestaurantOwnerMainPage::showNewOrderNotification(const QString& msg)
{
    auto *dialog = new QDialog(this);
    dialog->setWindowTitle("سفارش جدید");
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->resize(300, 150);

    QVBoxLayout* layout = new QVBoxLayout(dialog);
    QLabel* label = new QLabel(msg);
    label->setWordWrap(true);
    label->setAlignment(Qt::AlignRight);
    layout->addWidget(label);

    QPushButton* okButton = new QPushButton("باشه");
    layout->addWidget(okButton, 0, Qt::AlignCenter);

    connect(okButton, &QPushButton::clicked, dialog, &QDialog::accept);

    dialog->show();
}


void RestaurantOwnerMainPage::registerRestaurantSocket()
{
    QString restaurantName = currentOwner->getRestaurant().getName();
    qDebug() << "Registering restaurant socket:" << restaurantName;
    clientSocket->sendMessage("REGISTER_RESTAURANT_SOCKET:" + restaurantName);
}
