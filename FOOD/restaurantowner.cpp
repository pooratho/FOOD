#include "restaurantowner.h"

RestaurantOwner::RestaurantOwner(const QString& firstName,
                                         const QString& lastName,
                                         const QString& phone,
                                         const QString& password)
    : User(firstName, lastName, phone, password) {}


void RestaurantOwner::addRestaurant(const Restaurant& r) {
    ownedRestaurants.append(r);
}

QVector<Restaurant> RestaurantOwner::getRestaurants() const {
    return ownedRestaurants;
}
