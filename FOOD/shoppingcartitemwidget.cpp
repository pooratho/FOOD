#include "shoppingcartitemwidget.h"
#include "ui_shoppingcartitemwidget.h"


ShoppingCartItemWidget::ShoppingCartItemWidget(const CartItem& item, QWidget *parent)
    : QWidget(parent),
    ui(new Ui::ShoppingCartItemWidget),
    cartItem(item)
{
    ui->setupUi(this);
    updateUi();

    updatePriceDisplay();

    connect(ui->spinBoxQuantity, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ShoppingCartItemWidget::on_spinBoxQuantity_valueChanged);

    connect(ui->pushButton, &QPushButton::clicked,
            this, &ShoppingCartItemWidget::on_pushButton_clicked);

    connect(ui->spinBoxQuantity, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [=](int newVal) {
                emit quantityChanged(newVal);     // سیگنال به MainPage ارسال شود
                updatePriceDisplay();             // قیمت آیتم را به‌روز کن
            });

}

ShoppingCartItemWidget::~ShoppingCartItemWidget()
{
    delete ui;
}

CartItem ShoppingCartItemWidget::getCartItem() const
{
    return cartItem;
}

void ShoppingCartItemWidget::updateUi()
{
    ui->labelFoodName->setText("  " + cartItem.getFoodName());
    ui->labelRestaurantName->setText("  " + cartItem.getRestaurantName());
    ui->spinBoxQuantity->setValue(cartItem.getQuantity());
    ui->labelUnitPrice->setText("  " + QString::number(cartItem.getUnitPrice()) + " تومان");
}

void ShoppingCartItemWidget::on_spinBoxQuantity_valueChanged(int value)
{
    cartItem.setQuantity(value);
    emit quantityChanged(value);
}


void ShoppingCartItemWidget::on_pushButton_clicked()
{
    emit removeClicked(this);
}

void ShoppingCartItemWidget::updatePriceDisplay()
{
    int total = cartItem.getUnitPrice() * ui->spinBoxQuantity->value();
    ui->labelUnitPrice->setText(QString::number(total) + " تومان");
}

