#include "orderitemwidget.h"
#include "ui_orderitemwidget.h"

orderitemwidgett::orderitemwidgett(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::orderitemwidgett)
{
    ui->setupUi(this);
}

orderitemwidgett::~orderitemwidgett()
{
    delete ui;
}

void orderitemwidgett::setRestaurantName(const QString& name) {
    ui->labelRestaurant->setText("  " + name);
}

void orderitemwidgett::setFoodListText(const QString& text) {
    ui->labelFoods->setText(text);
}

// void orderitemwidgett::setTotalPrice(double price) {
//     ui->labelPrice->setText(QString::number(price) + " تومان");
// }

void orderitemwidgett::setStatus(const QString& status) {
    ui->labelPreparing->setStyleSheet("color: white; font-weight: normal;");
    ui->labelDelivering->setStyleSheet("color: white; font-weight: normal;");
    ui->labelDelivered->setStyleSheet("color: white; font-weight: normal;");

    if (status == "در حال آماده‌سازی") {
        ui->labelPreparing->setStyleSheet("color: orange; font-weight: bold;");
    }
    else if (status == "ارسال شد") {
        ui->labelDelivering->setStyleSheet("color: blue; font-weight: bold;");
    }
    else if (status == "تحویل داده شد") {
        ui->labelDelivered->setStyleSheet("color: green; font-weight: bold;");
    }
}
