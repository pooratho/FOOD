#include "restaurantowner.h"

RestaurantOwner::RestaurantOwner(const QString& firstName,
                                 const QString& lastName,
                                 const QString& phone,
                                 const QString& password,
                                 const Restaurant& restaurant,
                                 bool isBlocked)
    : User(firstName, lastName, phone, password, isBlocked),
    restaurant(restaurant)
{
}

Restaurant& RestaurantOwner::getRestaurant()  {
    return restaurant;
}

void RestaurantOwner::setRestaurant(const Restaurant& r) {
    restaurant = r;
}

QString RestaurantOwner::getRole() const {
    return "Restaurant";
}
