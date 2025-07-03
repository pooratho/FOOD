#include "restauranttablewidget.h"
#include "ui_restauranttablewidget.h"  // ✅ فایل تولید شده از UI

RestaurantTableWidget::RestaurantTableWidget(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::restauranttablewidget)  // ✅ استفاده از نام درست
{
    ui->setupUi(this);  // فراخوانی setupUi برای ایجاد رابط کاربری

    resize(601, 451);  // عرض و ارتفاع دلخواه
    ui->tableWidget->setColumnWidth(0, 100);   // شماره ردیف
    ui->tableWidget->setColumnWidth(1, 200);  // نام رستوران
    ui->tableWidget->setColumnWidth(2, 200);  // نام مالک
    ui->tableWidget->setColumnWidth(3, 150);  // وضعیت

    // همچنین ستون آخر را کشیدنی کن تا فضای اضافی گرفته شود
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);


}

RestaurantTableWidget::~RestaurantTableWidget()
{
    delete ui;
}
