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

        ui->label->setText("منوی رستوران: " + restaurantName);
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
            itemWidget->setPrice(price + " تومان");

            QListWidgetItem* listItem = new QListWidgetItem(list);
            listItem->setSizeHint(QSize(200, 80));  // تست سایز

            list->addItem(listItem);
            list->setItemWidget(listItem, itemWidget);
        } else {
            qDebug() << "⚠️ List is null for category:" << cat;
        }

    }
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


// void restaurantmenu::handleServerMessage(const QString& msg)
// {
//     if (msg.startsWith("MENU_LIST_FOR:")) {
//         ui->listWidgetMain->clear();
//         ui->listWidgetDessert->clear();
//         ui->listWidgetDrink->clear();
//         ui->listWidgetStarter->clear();
//         ui->listWidgetOthers->clear();

//         QString raw = msg.mid(QString("MENU_LIST_FOR:").length());
//         QStringList items = raw.split(";", Qt::SkipEmptyParts);

//         for (const QString& item : items) {
//             QStringList parts = item.split("|");
//             if (parts.size() != 4) continue;

//             QString name = parts[0];
//             QString desc = parts[1];
//             QString price = parts[2];
//             QString cat = parts[3];

//             QListWidget* list = nullptr;
//             if (cat == "غذا") list = ui->listWidgetMain;
//             else if (cat == "دسر") list = ui->listWidgetDessert;
//             else if (cat == "نوشیدنی") list = ui->listWidgetDrink;
//             else if (cat == "پیش غذا") list = ui->listWidgetStarter;
//             else list = ui->listWidgetOthers;

//             if (list) {
//                 RestaurantMenuItemWidget* itemWidget = new RestaurantMenuItemWidget(this);
//                 itemWidget->setName(name);
//                 itemWidget->setDescription(desc);
//                 itemWidget->setPrice(price + " تومان");

//                 QListWidgetItem* listItem = new QListWidgetItem(list);
//                 listItem->setSizeHint(itemWidget->sizeHint());

//                 list->addItem(listItem);
//                 list->setItemWidget(listItem, itemWidget);
//             }
//         }

//         ui->label->setText("منوی رستوران: " + restaurantName);
//     }
// }



