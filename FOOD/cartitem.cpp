#include "cartitem.h"

CartItem::CartItem(const QString& foodName,
                   const QString& restaurantName,
                   int quantity,
                   double unitPrice)
    : foodName(foodName),
    restaurantName(restaurantName),
    quantity(quantity),
    unitPrice(unitPrice) {}

QString CartItem::getFoodName() const {
    return foodName;
}

QString CartItem::getRestaurantName() const {
    return restaurantName;
}

int CartItem::getQuantity() const {
    return quantity;
}

double CartItem::getUnitPrice() const {
    return unitPrice;
}

double CartItem::getTotalPrice() const {
    return unitPrice * quantity;
}

void CartItem::setQuantity(int newQuantity) {
    quantity = newQuantity;
}
