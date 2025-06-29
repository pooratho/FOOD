#ifndef MENU_H
#define MENU_H

#include <QString>
#include <QVector>
#include "food.h"

class MenuItem
{
public:
    MenuItem();

    void addFood(const Food& food);
    void removeFood(const QString& foodName);
    QVector<Food> getFoods() const;
    Food* findFoodByName(const QString& name);

private:
    QVector<Food> foods;
};

#endif // MENU_H
