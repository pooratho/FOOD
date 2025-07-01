#ifndef RESTAURANTMENU_H
#define RESTAURANTMENU_H

#include <QWidget>
#include "clientsocketmanager.h"
#include "qlistwidget.h"
#include "cartitem.h"

namespace Ui {
class restaurantmenu;
}

class restaurantmenu : public QWidget
{
    Q_OBJECT

public:
          explicit restaurantmenu(const QString& restaurantName, const QString& customerPhone, QWidget *parent = nullptr);
    ~restaurantmenu();

    void collectSelectedItems();
private:
    Ui::restaurantmenu *ui;
    ClientSocketManager* clientSocket;
    QString restaurantName;  //  بهتر از currentRestaurantName
    void clearListWidgetCompletely(QListWidget* listWidget);
    QList<CartItem> cartItems;
      QString customerPhone;

private slots:
    void handleServerMessage(const QString& msg);
    void populateMenuItems(const QStringList& items);
    void on_pushButton_clicked();

signals:

    void requestShowCart();

      void cartItemsReady(const QList<CartItem>& cartItems);
};

#endif // RESTAURANTMENU_H
