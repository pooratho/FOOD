#include "restauranttablewidget.h"
#include "ui_restauranttablewidget.h"  // ✅ فایل تولید شده از UI

RestaurantTableWidget::RestaurantTableWidget(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::restauranttablewidget)  // ✅ استفاده از نام درست
{
    ui->setupUi(this);  // فراخوانی setupUi برای ایجاد رابط کاربری
}

RestaurantTableWidget::~RestaurantTableWidget()
{
    delete ui;
}
