#include "orderstablewidget.h"
#include "ui_orderstablewidget.h"

OrdersTableWidget::OrdersTableWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::orderstablewidget)
{
    ui->setupUi(this);

    // تنظیمات جدول
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    // تنظیم عرض ستون‌ها (تغییر بر اساس نیاز)
    ui->tableWidget->setColumnWidth(0, 120);
    ui->tableWidget->setColumnWidth(1, 150);
    ui->tableWidget->setColumnWidth(2, 100);
    ui->tableWidget->setColumnWidth(3, 80);
    ui->tableWidget->setColumnWidth(4, 150);
    ui->tableWidget->setColumnWidth(5, 120);

    // جلوگیری از تغییر اندازه ستون‌ها توسط کاربر
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
}

OrdersTableWidget::~OrdersTableWidget()
{
    delete ui;
}
