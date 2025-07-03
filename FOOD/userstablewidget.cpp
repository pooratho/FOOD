#include "userstablewidget.h"
#include "ui_userstablewidget.h"

UserTableWidget::UserTableWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::usertablewidget)      // ← نام دقیق همان است که uic تولید کرده
{
    ui->setupUi(this);                 // ویجت‌های .ui را می‌سازد

    /* تنظیمات خواسته‌شده */
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->tableWidget->setColumnWidth(0, 200);  // ستون اول
    ui->tableWidget->setColumnWidth(1, 200);  // ستون دوم
    ui->tableWidget->setColumnWidth(2, 187);  // ستون سوم

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
}

UserTableWidget::~UserTableWidget()
{
    delete ui;
}
