#ifndef CUSTOMERMAINPAGE_H
#define CUSTOMERMAINPAGE_H

#include <QWidget>
#include <QMap>
#include <QStringList>
#include <QTableWidgetItem>

#include "customer.h"
#include "clientsocketmanager.h"  // برای اتصال به سرور
#include "cartitem.h"
#include "shoppingcartitemwidget.h"

namespace Ui {
class CustomerMainPage;
}

class CustomerMainPage : public QWidget
{
    Q_OBJECT

public:
    explicit CustomerMainPage(Customer* customer, QWidget *parent = nullptr);
    ~CustomerMainPage();

private slots:
    void on_comboBox_2_currentTextChanged(const QString &province);
    void on_comboBox_3_currentTextChanged(const QString &city);

    void handleServerMessage(const QString &msg); //  اسلات برای گرفتن پیام‌های سرور
    void sendFilteredRequest();

    void onTableItemDoubleClicked(int row, int column);

    //void handleCartItemsFromRestaurant(const QList<CartItem>& items);

signals:
    void foodSelectedForCart(const CartItem& item);

private:
    Ui::CustomerMainPage *ui;
    QMap<QString, QStringList> provinceCitiesMap;
    Customer* customer;

    QList<CartItem*> cartItems;

    ClientSocketManager* clientSocket;

    void updateCartDisplay();
    void removeCartItem(ShoppingCartItemWidget* widget);
    void updateTotalPriceDisplay();
    bool isFoodInCart(const QString& restaurantName, const QString& foodName) const;
};

#endif // CUSTOMERMAINPAGE_H
