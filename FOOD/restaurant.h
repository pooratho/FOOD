#ifndef RESTAURANT_H
#define RESTAURANT_H

#include <QString>
#include <QVector>
#include "menuitem.h"

class Restaurant
{
public:
    Restaurant(int id,
               const QString& name,
               const QString& ownerUsername,
               const QString& province,
               const QString& city,
               bool isActive = true);

    int getId() const;
    QString getName() const;
    QString getOwnerUsername() const;
    QString getProvince() const;
    QString getCity() const;
    bool getIsActive() const;
Restaurant(const QString& name, const QString& province, const QString& city);
    void setActive(bool active);
    void addMenuItem(const MenuItem& item);
    QVector<MenuItem> getMenu() const;

private:
    int id;
    QString name;
    QString ownerUsername;
    QString province;
    QString city;
    bool isActive;

    QVector<MenuItem> menu;
};

#endif // RESTAURANT_H
