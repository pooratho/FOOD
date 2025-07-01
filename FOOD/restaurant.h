#ifndef RESTAURANT_H
#define RESTAURANT_H

#include <QString>
#include <QVector>
#include <QMap>               // ⬅️ این خطو اضافه کن
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

    Restaurant(const QString& name, const QString& province, const QString& city);  // اینو بهتر بیاری بالا

    int getId() const;
    QString getName() const;
    QString getOwnerUsername() const;
    QString getProvince() const;
    QString getCity() const;
    bool getIsActive() const;

    void setActive(bool active);
    void addMenuItem(const QString& category, const Food& food);
    QMap<QString, MenuItem>& getMenu() ;
    void setName(const QString& newName) ;
private:
    int id;
    QString name;
    QString ownerUsername;
    QString province;
    QString city;
    bool isActive;

    QMap<QString, MenuItem> menu;
};

#endif // RESTAURANT_H
