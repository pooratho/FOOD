#include "restaurantmenuitemwidget.h"
#include "ui_restaurantmenuitemwidget.h"

RestaurantMenuItemWidget::RestaurantMenuItemWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::restaurantmenuitemwidget)
{
    ui->setupUi(this);
}

RestaurantMenuItemWidget::~RestaurantMenuItemWidget()
{
    delete ui;
}

void RestaurantMenuItemWidget::setName(const QString &name)
{
    ui->name->setText("  " + name);
}

void RestaurantMenuItemWidget::setDescription(const QString &desc)
{
    ui->description->setText("  " + desc);
}

void RestaurantMenuItemWidget::setPrice(const QString &price)
{
    ui->price->setText(price);
}

bool RestaurantMenuItemWidget::isSelected() const
{
    return ui->checkBox->isChecked();
}
