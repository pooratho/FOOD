#include "restaurantowner.h"

RestaurantOwner::RestaurantOwner(const QString& firstName,
                                 const QString& lastName,
                                 const QString& phone,
                                 const QString& password,
                                 const Restaurant& restaurant)
    : User(firstName, lastName, phone, password),
    restaurant(restaurant)
{
}

Restaurant RestaurantOwner::getRestaurant() const {
    return restaurant;
}

void RestaurantOwner::setRestaurant(const Restaurant& r) {
    restaurant = r;
}

QString RestaurantOwner::getRole() const {
    return "Restaurant";
}
