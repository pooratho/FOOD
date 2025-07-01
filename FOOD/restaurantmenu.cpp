#include "restaurantmenu.h"
#include "qtimer.h"
#include "ui_restaurantmenu.h"
#include "restaurantmenuitemwidget.h"
#include <QDebug>

restaurantmenu::restaurantmenu(const QString& restaurantName, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::restaurantmenu)
    , restaurantName(restaurantName)
{
    ui->setupUi(this);

    clientSocket = new ClientSocketManager(this);

    connect(clientSocket, &ClientSocketManager::connected, this, [=]() {
        qDebug() << "📡 Connected to server from RestaurantMenu";
        clientSocket->sendMessage("REGISTER_RESTAURANT_SOCKET:" + restaurantName);
        QTimer::singleShot(200, this, [=]() {
            clientSocket->sendMessage("GET_MENU");
        });
    });

    connect(ui->pushButton, &QPushButton::clicked, this, [=]() {
        emit requestShowCart();  // اطلاع دادن به صفحه اصلی
        this->close();           // بستن صفحه منو
    });

    connect(clientSocket, &ClientSocketManager::messageReceived,
            this, &restaurantmenu::handleServerMessage);

    clientSocket->connectToServer("127.0.0.1", 1234);
}

restaurantmenu::~restaurantmenu()
{
    delete ui;
}

void restaurantmenu::handleServerMessage(const QString& msg)
{
    if (msg.startsWith("MENU_LIST:")) {
        QString raw = msg.mid(QString("MENU_LIST:").length());

        if (raw.trimmed().isEmpty()) {
            qDebug() << "Empty menu received.";
            return;
        }

        QStringList items = raw.split(";", Qt::SkipEmptyParts);
        populateMenuItems(items);

        ui->label->setText("  رستوران: " + restaurantName);
    }
}

void restaurantmenu::populateMenuItems(const QStringList& items)
{
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
        QString price = parts[2];
        QString cat = parts[3];

        QListWidget* list = nullptr;
        if (cat == "غذا") list = ui->listWidgetMain;
        else if (cat == "دسر") list = ui->listWidgetDessert;
        else if (cat == "نوشیدنی") list = ui->listWidgetDrink;
        else if (cat == "پیش غذا") list = ui->listWidgetStarter;
        else list = ui->listWidgetOthers;

        if (list) {
            qDebug() << "✅ Adding to list:" << cat << name;

            RestaurantMenuItemWidget* itemWidget = new RestaurantMenuItemWidget(this);
            itemWidget->setName(name);
            itemWidget->setDescription(desc);
            itemWidget->setPrice("  " + price + " تومان");

            QListWidgetItem* listItem = new QListWidgetItem(list);
            listItem->setSizeHint(QSize(610, 100));  // تست سایز

            list->addItem(listItem);
            list->setItemWidget(listItem, itemWidget);
        } else {
            qDebug() << "⚠️ List is null for category:" << cat;
        }

    }

    ui->label_5->setVisible(ui->listWidgetMain->count() == 0);
    ui->label_9->setVisible(ui->listWidgetDessert->count() == 0);
    ui->label_8->setVisible(ui->listWidgetDrink->count() == 0);
    ui->label_7->setVisible(ui->listWidgetStarter->count() == 0);
    ui->label_6->setVisible(ui->listWidgetOthers->count() == 0);

}

void restaurantmenu::clearListWidgetCompletely(QListWidget* listWidget)
{
    while (listWidget->count() > 0) {
        QListWidgetItem* item = listWidget->takeItem(0);
        if (item) {
            QWidget* widget = listWidget->itemWidget(item);
            if (widget) widget->deleteLater();
            delete item;
        }
    }
}




// void restaurantmenu::collectSelectedItems()
// {
//     cartItems.clear();

//     QList<QListWidget*> allLists = {
//         ui->listWidgetMain,
//         ui->listWidgetDessert,
//         ui->listWidgetDrink,
//         ui->listWidgetStarter,
//         ui->listWidgetOthers
//     };

//     for (QListWidget* list : allLists) {
//         for (int i = 0; i < list->count(); ++i) {
//             QListWidgetItem* listItem = list->item(i);
//             RestaurantMenuItemWidget* itemWidget = qobject_cast<RestaurantMenuItemWidget*>(list->itemWidget(listItem));
//             if (itemWidget && itemWidget->isSelected()) {
//                 QString foodName = itemWidget->getName();
//                 QString restaurant = restaurantName; // متغیر عضو کلاس restaurantmenu
//                 int quantity = 1; // فعلاً یک عدد، بعداً میتونی توسعه بدی
//                 double unitPrice = itemWidget->getPriceValue();

//                 // بررسی اینکه اگر قبلا همین غذا تو سبد هست، تعداد رو افزایش بده
//                 bool found = false;
//                 for (CartItem& ci : cartItems) {
//                     if (ci.getFoodName() == foodName && ci.getRestaurantName() == restaurant) {
//                         ci.setQuantity(ci.getQuantity() + quantity);
//                         found = true;
//                         break;
//                     }
//                 }

//                 if (!found) {
//                     cartItems.append(CartItem(foodName, restaurant, quantity, unitPrice));
//                 }
//             }
//         }
//     }
// }


// void restaurantmenu::on_pushButton_clicked()
// {
//     collectSelectedItems();  // همون که قبلاً گفتیم آیتم‌های تیک‌خورده رو جمع می‌کنه

//     emit cartItemsReady(cartItems);
// }


void restaurantmenu::on_pushButton_clicked()
{

}

