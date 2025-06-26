#include "restaurantownersignupwindow.h"
#include "ui_restaurantOwnerSignUp.h"  // توجه کن اسم فایل هدر uic همین است

RestaurantOwnerSignUpWindow::RestaurantOwnerSignUpWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
}

RestaurantOwnerSignUpWindow::~RestaurantOwnerSignUpWindow()
{
    delete ui;
}
