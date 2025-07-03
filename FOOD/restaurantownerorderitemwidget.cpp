#include "restaurantownerorderitemwidget.h"
#include "ui_restaurantownerorderitemwidget.h"

RestaurantOwnerOrderItemWidget::RestaurantOwnerOrderItemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::restaurantownerorderitemwidget)
{
    ui->setupUi(this);

    setupStatusComboBox();

    // وصل کردن سیگنال تغییر کامبوباکس به اسلات ما
    connect(ui->comboBoxStatus, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &RestaurantOwnerOrderItemWidget::onStatusChanged);
}

RestaurantOwnerOrderItemWidget::~RestaurantOwnerOrderItemWidget()
{
    delete ui;
}

void RestaurantOwnerOrderItemWidget::setupStatusComboBox()
{
    ui->comboBoxStatus->clear();
    ui->comboBoxStatus->addItems({"ثبت شده", "در حال آماده‌سازی", "ارسال شده", "تحویل داده شده"});
}

void RestaurantOwnerOrderItemWidget::setStatus(const QString &status)
{
    int index = ui->comboBoxStatus->findText(status);
    if (index >= 0)
        ui->comboBoxStatus->setCurrentIndex(index);
    else
        ui->comboBoxStatus->setCurrentIndex(0);
}

void RestaurantOwnerOrderItemWidget::onStatusChanged(int index)
{
    QString newStatus = ui->comboBoxStatus->itemText(index);
    emit updateStatusRequested(newStatus);
}


void RestaurantOwnerOrderItemWidget::setRestaurantName(const QString &name)
{
    ui->labelRestaurantName->setText(name);
}

void RestaurantOwnerOrderItemWidget::setFoodText(const QString &foods)
{
    ui->labelFoodInfo->setText(foods);
}

void RestaurantOwnerOrderItemWidget::setCustomerPhone(const QString &phone)
{
    ui->labelCustomerPhone->setText(phone);
}

