#include "restauranttablewidget.h"
#include "ui_restauranttablewidget.h"
#include<QMessageBox>

RestaurantTableWidget::RestaurantTableWidget(ClientSocketManager* socketManager, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::restauranttablewidget)
    , clientSocket(socketManager)
{
    ui->setupUi(this);

    /* 1) اگر می‌خواهی پنجره تغییر اندازه نداشته باشد: */
    setFixedSize(601, 451);      // ← پنجره قفل می‌شود
    // اگر فقط می‌خواهی اندازۀ اولیه باشد ولی کاربر بتواند بزرگ کند:
    // resize(601,451);

    /* 2) ستون‌ها را قفل و عرض را مشخص کن */
    ui->tableWidget->setColumnCount(5);  // اضافه کن این خط رو

    // سپس عرض ستون‌ها رو تنظیم کن:
    ui->tableWidget->setColumnWidth(0, 50);   // ستون ID
    ui->tableWidget->setColumnWidth(1, 150);  // نام رستوران
    ui->tableWidget->setColumnWidth(2, 150);  // نام مالک
    ui->tableWidget->setColumnWidth(3, 170);  // آدرس
    ui->tableWidget->setColumnWidth(4, 100);  // وضعیت

    auto *header = ui->tableWidget->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Fixed);
    header->setSectionsMovable(false);
    header->setSectionsClickable(false);
    header->setSectionResizeMode(4, QHeaderView::Stretch);
}

RestaurantTableWidget::~RestaurantTableWidget()
{
    delete ui;
}

void RestaurantTableWidget::on_pushButton_2_clicked()
{
    QTableWidget* table = ui->tableWidget;
    if (!table) {
        QMessageBox::warning(this, "خطا", "جدول رستوران‌ها پیدا نشد.");
        return;
    }

    auto selectedRanges = table->selectedRanges();
    if (selectedRanges.isEmpty()) {
        QMessageBox::warning(this, "خطا", "یک رستوران را انتخاب کنید.");
        return;
    }

    int row = selectedRanges.first().topRow();

    QTableWidgetItem* idItem = table->item(row, 0);      // ستون ID
    QTableWidgetItem* statusItem = table->item(row, 4);  // ستون وضعیت (حالا ستون ۴)

    if (!idItem || !statusItem) {
        QMessageBox::warning(this, "خطا", "اطلاعات رستوران ناقص است.");
        return;
    }

    QString restaurantId = idItem->text();
    QString currentStatus = statusItem->text().toLower();

    QString newStatus;
    if (currentStatus == "active" || currentStatus == "فعال") {
        newStatus = "inactive";  // یا "غیرفعال"
    } else {
        newStatus = "active";    // یا "فعال"
    }

    QString msg = QString("TOGGLE_RESTAURANT_STATUS:%1:%2\n").arg(restaurantId).arg(newStatus);
  qDebug() << "Sending to server:" << msg;
    if (clientSocket) {
        qDebug() << "Sending to server:" << msg;
        clientSocket->sendMessage(msg);
    } else {
        QMessageBox::warning(this, "خطا", "ارتباط با سرور برقرار نیست.");
    }
}

