#include "shoppingcartitemwidget.h"
#include "ui_shoppingcartitemwidget.h"

ShoppingCartItemWidget::ShoppingCartItemWidget(const CartItem& item, QWidget *parent)
    : QWidget(parent),
    ui(new Ui::ShoppingCartItemWidget),
    cartItem(item)
{
    ui->setupUi(this);
    updateUi();

    connect(ui->spinBoxQuantity, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ShoppingCartItemWidget::on_spinBoxQuantity_valueChanged);

    connect(ui->pushButton, &QPushButton::clicked,
            this, &ShoppingCartItemWidget::on_pushButton_clicked);
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
    ui->labelFoodName->setText(cartItem.getFoodName());
    ui->labelRestaurantName->setText(cartItem.getRestaurantName());
    ui->spinBoxQuantity->setValue(cartItem.getQuantity());
    ui->labelUnitPrice->setText(QString::number(cartItem.getUnitPrice()) + " تومان");
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

