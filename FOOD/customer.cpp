#include "customer.h"

Customer::Customer(const QString& firstName,
                   const QString& lastName,
                   const QString& phone,
                   const QString& password,
                   bool isBlocked)
    : User(firstName, lastName, phone, password, isBlocked) {}  // ➕ تغییر در کانستراکتور پایه

QString Customer::getRole() const {
    return "Customer";
}

void Customer::addToCart(const CartItem& item) {
    cart.append(item);
}

void Customer::clearCart() {
    cart.clear();
}

QVector<CartItem> Customer::getCart() const {
    return cart;
}

void Customer::addOrder(const Order& order) {
    orderHistory.append(order);
}

QVector<Order> Customer::getOrderHistory() const {
    return orderHistory;
}
