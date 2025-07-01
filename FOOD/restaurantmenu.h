#ifndef RESTAURANTMENU_H
#define RESTAURANTMENU_H

#include <QWidget>
#include "clientsocketmanager.h"

namespace Ui {
class restaurantmenu;
}

class restaurantmenu : public QWidget
{
    Q_OBJECT

public:
    explicit restaurantmenu(const QString& restaurantName, QWidget *parent = nullptr);
    ~restaurantmenu();

private:
    Ui::restaurantmenu *ui;
    ClientSocketManager* clientSocket;
    QString restaurantName;  // ✅ بهتر از currentRestaurantName

private slots:
    void handleServerMessage(const QString& msg);
};

#endif // RESTAURANTMENU_H
