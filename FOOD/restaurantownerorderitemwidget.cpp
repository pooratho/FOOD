#include "restaurantownerorderitemwidget.h"

RestaurantOwnerOrderItemWidget::RestaurantOwnerOrderItemWidget(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);          // مهم‌ترین خط: المان‌هاى UI را به ‌this مى‌چسباند
}

/* ---------- متدهاى کمکی ---------- */

void RestaurantOwnerOrderItemWidget::setRestaurantName(const QString &name)
{
    ui.restaurantName->setText(name);
}

void RestaurantOwnerOrderItemWidget::setCustomerName(const QString &name)
{
    ui.customerName->setText(name);
}

void RestaurantOwnerOrderItemWidget::setFoodText(const QString &foods)
{
    ui.foodText->setText(foods);
}

void RestaurantOwnerOrderItemWidget::setStatus(const QString &status)
{
    ui.comboBox->setCurrentText(status);
}


