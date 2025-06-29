#ifndef RESTAURANTOWNERMAINPAGE_H
#define RESTAURANTOWNERMAINPAGE_H

#include <QWidget>
#include "ui_restaurantOwnerMainPage.h"  // فایل تولید شده توسط uic

class RestaurantOwnerMainPage : public QWidget
{
    Q_OBJECT

public:
    explicit RestaurantOwnerMainPage(QWidget *parent = nullptr);
    ~RestaurantOwnerMainPage();

private:
    Ui::restaurantOwnerMainPage ui;  // شی UI که همه ویجت‌ها را دارد
};

#endif // RESTAURANTOWNERMAINPAGE_H
