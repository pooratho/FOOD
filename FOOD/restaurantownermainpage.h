#ifndef RESTAURANTOWNERMAINPAGE_H
#define RESTAURANTOWNERMAINPAGE_H

#include <QWidget>
#include "restaurantowner.h"  // برای استفاده از کلاس رستوران‌دار

namespace Ui {
class restaurantOwnerMainPage;
}

class RestaurantOwnerMainPage : public QWidget
{
    Q_OBJECT

public:
    explicit RestaurantOwnerMainPage(RestaurantOwner* owner, QWidget *parent = nullptr);
    ~RestaurantOwnerMainPage();

private:
    Ui::restaurantOwnerMainPage *ui;
    RestaurantOwner* currentOwner;  // نگه داشتن شی رستوران‌دار
};

#endif // RESTAURANTOWNERMAINPAGE_H
