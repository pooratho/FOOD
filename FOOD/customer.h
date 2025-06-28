#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "user.h"
#include <QString>
#include <QVector>
#include "order.h"

class Order;     // به‌زودی تعریف می‌شه
class CartItem;  // به‌زودی تعریف می‌شه

class Customer : public User {
public:
    Customer(const QString& firstName,
             const QString& lastName,
             const QString& phone,
             const QString& password);

    QString getRole() const override;

    // قابلیت‌های آینده
    void addToCart(const CartItem& item);
    void clearCart();
    QVector<CartItem> getCart() const;

    void addOrder(const Order& order);
    QVector<Order> getOrderHistory() const;

private:
    QVector<CartItem> cart;
    QVector<Order> orderHistory;
};

#endif // CUSTOMER_H
