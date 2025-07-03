// orderitemwidget.cpp
#include "orderitemwidget.h"
#include "ui_orderitemwidget.h"

orderitemwidget::orderitemwidget(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::orderitemwidget)
{
    ui->setupUi(this);
}

orderitemwidget::~orderitemwidget()
{
    delete ui;
}

void orderitemwidget::setRestaurantName(const QString& name) {
    ui->labelRestaurant->setText("  " + name);
}

void orderitemwidget::setFoodListText(const QString& text) {
    ui->labelFoods->setText(text);
}

// void orderitemwidgett::setTotalPrice(double price) {
//     ui->labelPrice->setText(QString::number(price) + " تومان");
// }

void orderitemwidget::setStatus(const QString& status) {
    ui->labelPreparing->setStyleSheet("color: white; font-weight: normal;");
    ui->labelDelivering->setStyleSheet("color: white; font-weight: normal;");
    ui->labelDelivered->setStyleSheet("color: white; font-weight: normal;");

    if (status == "در حال آماده‌سازی") {
        ui->labelPreparing->setStyleSheet("color: orange; font-weight: bold;");
    }
    else if (status == "ارسال شده") {
        ui->labelPreparing->setStyleSheet("color: orange; font-weight: bold;");

        ui->labelDelivering->setStyleSheet("color: blue; font-weight: bold;");
    }
    else if (status == "تحویل داده شده") {
        ui->labelPreparing->setStyleSheet("color: orange; font-weight: bold;");

        ui->labelDelivering->setStyleSheet("color: blue; font-weight: bold;");

        ui->labelDelivered->setStyleSheet("color: green; font-weight: bold;");
    }
}

void orderitemwidget::setOrderId(int id) { orderId = id; }
int orderitemwidget::getOrderId() const  { return orderId; }
