#ifndef CARTITEM_H
#define CARTITEM_H

#include <QString>

class CartItem {
public:
    CartItem(const QString& foodName,
             const QString& restaurantName,
             int quantity,
             double unitPrice);

    QString getFoodName() const;
    QString getRestaurantName() const;
    int getQuantity() const;
    double getUnitPrice() const;
    double getTotalPrice() const;

    void setQuantity(int newQuantity);

private:
    QString foodName;
    QString restaurantName;
    int quantity;
    double unitPrice;
};

#endif // CARTITEM_H
