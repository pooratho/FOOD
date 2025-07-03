#include "restauranttablewidget.h"
#include "ui_restauranttablewidget.h"

RestaurantTableWidget::RestaurantTableWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::restauranttablewidget)
{
    ui->setupUi(this);

    /* 1) اگر می‌خواهی پنجره تغییر اندازه نداشته باشد: */
    setFixedSize(601, 451);      // ← پنجره قفل می‌شود
    // اگر فقط می‌خواهی اندازۀ اولیه باشد ولی کاربر بتواند بزرگ کند:
    // resize(601,451);

    /* 2) ستون‌ها را قفل و عرض را مشخص کن */
    auto *header = ui->tableWidget->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Fixed);  // تمام ستون‌ها ثابت

    ui->tableWidget->setColumnWidth(0, 150);  // نام رستوران
    ui->tableWidget->setColumnWidth(1, 150);  // نام مالک
    ui->tableWidget->setColumnWidth(2, 170);  // آدرس
    ui->tableWidget->setColumnWidth(3, 100);  // وضعیت

    /* 3) اجازه نده ستون‌ها جابه‌جا یا دوبل‌کلیک resize شوند */
    header->setSectionsMovable(false);
    header->setSectionsClickable(false);

    /* 4) اگر می‌خواهی فضای خالیِ اضافی گرفته شود، ستون آخر را Stretch کن
          (ولی همچنان قابل درَگ نخواهد بود) */
    header->setSectionResizeMode(3, QHeaderView::Stretch);
}

RestaurantTableWidget::~RestaurantTableWidget()
{
    delete ui;
}
