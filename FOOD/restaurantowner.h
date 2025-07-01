#ifndef RESTAURANTOWNER_H
#define RESTAURANTOWNER_H

#include "user.h"
#include "restaurant.h"

class RestaurantOwner : public User
{
private:
    Restaurant restaurant;

public:
    RestaurantOwner(const QString& firstName,
                    const QString& lastName,
                    const QString& phone,
                    const QString& password,
                    const Restaurant& restaurant,
                    bool isBlocked = false); // ➕ این پارامتر اضافه شد

    Restaurant getRestaurant() const;
    void setRestaurant(const Restaurant& r);

    QString getRole() const override;
};

#endif // RESTAURANTOWNER_H
