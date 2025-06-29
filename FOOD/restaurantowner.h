#ifndef RESTAURANTOWNER_H
#define RESTAURANTOWNER_H

#include "user.h"
#include "restaurant.h"

class RestaurantOwner : public User
{
public:
    RestaurantOwner(const QString& firstName,
                    const QString& lastName,
                    const QString& phone,
                    const QString& password);

    void addRestaurant(const Restaurant& r);
    QVector<Restaurant> getRestaurants() const;

private:
    QVector<Restaurant> ownedRestaurants;
};

#endif // RESTAURANTOWNER_H
