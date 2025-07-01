#include "restaurantmenu.h"
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
        clientSocket->sendMessage("GET_MENU_FOR:" + restaurantName);
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
    if (msg.startsWith("MENU_LIST_FOR:")) {
        ui->listWidgetMain->clear();
        ui->listWidgetDessert->clear();
        ui->listWidgetDrink->clear();
        ui->listWidgetStarter->clear();
        ui->listWidgetOthers->clear();

        QString raw = msg.mid(QString("MENU_LIST_FOR:").length());
        QStringList items = raw.split(";", Qt::SkipEmptyParts);

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
                RestaurantMenuItemWidget* itemWidget = new RestaurantMenuItemWidget(this);
                itemWidget->setName(name);
                itemWidget->setDescription(desc);
                itemWidget->setPrice(price + " تومان");

                QListWidgetItem* listItem = new QListWidgetItem(list);
                listItem->setSizeHint(itemWidget->sizeHint());

                list->addItem(listItem);
                list->setItemWidget(listItem, itemWidget);
            }
        }

        ui->label->setText("منوی رستوران: " + restaurantName);
    }
}



