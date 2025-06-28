#include "order.h"

Order::Order(int orderId,
             const QString& customerUsername,
             const QVector<CartItem>& items,
             QDateTime orderTime)
    : orderId(orderId),
    customerUsername(customerUsername),
    items(items),
    orderTime(orderTime),
    status(Status::Pending) {}

int Order::getOrderId() const {
    return orderId;
}

QString Order::getCustomerUsername() const {
    return customerUsername;
}

QVector<CartItem> Order::getItems() const {
    return items;
}

double Order::getTotalPrice() const {
    double total = 0;
    for (const auto& item : items) {
        total += item.getTotalPrice();
    }
    return total;
}

QDateTime Order::getOrderTime() const {
    return orderTime;
}

Order::Status Order::getStatus() const {
    return status;
}

void Order::setStatus(Status newStatus) {
    status = newStatus;
}

QString Order::statusToString() const {
    switch (status) {
    case Status::Pending: return "در انتظار تأیید";
    case Status::Preparing: return "در حال آماده‌سازی";
    case Status::Delivering: return "در حال ارسال";
    case Status::Delivered: return "تحویل شده";
    case Status::Cancelled: return "لغو شده";
    default: return "نامشخص";
    }
}
