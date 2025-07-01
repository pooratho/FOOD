#include "restaurantmenu.h"
#include "ui_restaurantmenu.h"
#include <QDebug>

restaurantmenu::restaurantmenu(const QString& restaurantName, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::restaurantmenu)
    , currentRestaurantName(restaurantName)
{
    ui->setupUi(this);

    clientSocket = new ClientSocketManager(this);

    connect(clientSocket, &ClientSocketManager::connected, this, [=]() {
        qDebug() << "📡 Connected to server from RestaurantMenu";
        clientSocket->sendMessage("GET_MENU_FOR:" + currentRestaurantName);
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
        QString raw = msg.mid(QString("MENU_LIST_FOR:").length());
        QStringList items = raw.split(";", Qt::SkipEmptyParts);

        for (const QString& item : items) {
            QStringList parts = item.split("|");
            if (parts.size() != 4) continue;

            QString name = parts[0];
            QString desc = parts[1];
            QString price = parts[2];
            QString cat = parts[3];

            // مثلاً اضافه کن به تب مناسب در UI
            QListWidget* list = nullptr;
            if (cat == "غذا") list = ui->listWidgetMain;
            else if (cat == "دسر") list = ui->listWidgetDessert;
            else if (cat == "نوشیدنی") list = ui->listWidgetDrink;
            else if (cat == "پیش غذا") list = ui->listWidgetStarter;
            else list = ui->listWidgetOthers;

            if (list) {
                QListWidgetItem* item = new QListWidgetItem(name + " - " + price + " تومان\n" + desc);
                list->addItem(item);
            }
        }

        ui->label->setText("منوی رستوران: " + currentRestaurantName);
    }
}
