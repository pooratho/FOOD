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

void RestaurantOwnerMenuItemWidget::setItemInfo(const QString &name, const QString &description, const QString &priceText)
{
    ui->name->setText(name);
    ui->description->setText(description);
    ui->price->setText(priceText);
}

void RestaurantOwnerMenuItemWidget::on_pushButton_clicked()
{
    emit removeRequested(); // وقتی کاربر روی دکمه حذف میزنه
}

