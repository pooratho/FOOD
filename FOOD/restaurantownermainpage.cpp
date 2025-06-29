#include "restaurantownermainpage.h"

RestaurantOwnerMainPage::RestaurantOwnerMainPage(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);  // اتصال UI به این ویجت
}

RestaurantOwnerMainPage::~RestaurantOwnerMainPage()
{
    // معمولاً نیازی به کد خاص نیست چون Qt خودش پاکسازی می‌کند
}
