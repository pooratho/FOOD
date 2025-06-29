#ifndef FOOD_H
#define FOOD_H

#include <QString>

class Food
{
public:
    Food();
    Food(const QString &name, const QString &description, double price, const QString &category);

    QString getName() const;
    QString getDescription() const;
    double getPrice() const;
    QString getCategory() const;

    void setName(const QString &name);
    void setDescription(const QString &description);
    void setPrice(double price);
    void setCategory(const QString &category);

private:
    QString name;
    QString description;
    double price;
    QString category; // نوع غذا
};

#endif // FOOD_H
