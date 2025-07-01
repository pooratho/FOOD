#include "restaurant.h"

Restaurant::Restaurant(int id, const QString& name, const QString& ownerUsername,
                       const QString& province, const QString& city, bool isActive)
    : id(id), name(name), ownerUsername(ownerUsername),
    province(province), city(city), isActive(isActive) {}

Restaurant::Restaurant(const QString& name, const QString& province, const QString& city)
    : id(-1), name(name), ownerUsername(""), province(province), city(city), isActive(true) {}

int Restaurant::getId() const {
    return id;
}

QString Restaurant::getName() const {
    return name;
}

QString Restaurant::getOwnerUsername() const {
    return ownerUsername;
}

QString Restaurant::getProvince() const {
    return province;
}

QString Restaurant::getCity() const {
    return city;
}

bool Restaurant::getIsActive() const {
    return isActive;
}

void Restaurant::setActive(bool active) {
    isActive = active;
}

void Restaurant::addMenuItem(const QString& category, const Food& food) {
    menu[category].addFood(food);  // اگر وجود نداشته باشه، خودش ایجادش می‌کنه
}

QMap<QString, MenuItem>& Restaurant::getMenu() {
    return menu;
}

