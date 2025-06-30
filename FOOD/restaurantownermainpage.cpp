#include "restaurantownermainpage.h"
#include "ui_restaurantownermainpage.h"

RestaurantOwnerMainPage::RestaurantOwnerMainPage(RestaurantOwner* owner, QWidget *parent)
    : QWidget(parent),
    ui(new Ui::restaurantOwnerMainPage),
    currentOwner(owner)
{
    ui->setupUi(this);


}

RestaurantOwnerMainPage::~RestaurantOwnerMainPage()
{
    delete ui;
}
