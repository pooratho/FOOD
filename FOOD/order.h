#ifndef ORDER_H
#define ORDER_H

#include <QString>
#include <QDateTime>
#include <QVector>
#include "cartitem.h"

class Order
{
public:
    enum class Status {
        Pending,
        Preparing,
        Delivering,
        Delivered,
        Cancelled
    };

    Order(int orderId,
          const QString& customerUsername,
          const QVector<CartItem>& items,
          QDateTime orderTime = QDateTime::currentDateTime());

    int getOrderId() const;
    QString getCustomerUsername() const;
    QVector<CartItem> getItems() const;
    double getTotalPrice() const;
    QDateTime getOrderTime() const;
    Status getStatus() const;

    void setStatus(Status newStatus);
    QString statusToString() const;

private:
    int orderId;
    QString customerUsername;
    QVector<CartItem> items;
    QDateTime orderTime;
    Status status;
};

#endif // ORDER_H
