#include "menuitem.h"

MenuItem::MenuItem() {}

void MenuItem::addFood(const Food &food) {
    foods.append(food);
}

void MenuItem::removeFood(const QString &foodName) {
    for (int i = 0; i < foods.size(); ++i) {
        if (foods[i].getName() == foodName) {
            foods.remove(i);
            break;
        }
    }
}

QVector<Food> MenuItem::getFoods() const {
    return foods;
}

Food* MenuItem::findFoodByName(const QString &name) {
    for (int i = 0; i < foods.size(); ++i) {
        if (foods[i].getName() == name) {
            return &foods[i];
        }
    }
    return nullptr;
}
