#include "restaurantownermenuitemwidget.h"
#include "ui_restaurantownermenuitemwidget.h"

RestaurantOwnerMenuItemWidget::RestaurantOwnerMenuItemWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::restaurantownermenuitemwidget)
{
    ui->setupUi(this);

    connect(ui->pushButton, &QPushButton::clicked, this, &RestaurantOwnerMenuItemWidget::on_pushButton_clicked);
}

RestaurantOwnerMenuItemWidget::~RestaurantOwnerMenuItemWidget()
{
    delete ui;
}
void RestaurantOwnerMenuItemWidget::setItemInfo(const Food& food)
{
    currentFood = food;
    ui->name->setText("  " + food.getName());
    ui->description->setText("  " + food.getDescription());
    ui->price->setText(QString::number(food.getPrice()));
}


void RestaurantOwnerMenuItemWidget::on_pushButton_clicked()
{
    emit removeRequested(); // وقتی کاربر روی دکمه حذف میزنه
}
void RestaurantOwnerMenuItemWidget::setFood(const Food& food)
{
    currentFood = food;
  setItemInfo(food);
}

Food RestaurantOwnerMenuItemWidget::getFood() const
{
    return currentFood;
}

