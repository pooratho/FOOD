#include "shoppingcartitemwidget.h"
#include "ui_shoppingcartitemwidget.h"
<<<<<<< HEAD

ShoppingCartItemWidget::ShoppingCartItemWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::shoppingcartitemwidget)
{
    ui->setupUi(this);

    // اتصال سیگنال دکمه حذف به اسلات مربوطه
    connect(ui->pushButton, &QPushButton::clicked, this, &ShoppingCartItemWidget::onRemoveClicked);
}

ShoppingCartItemWidget::~ShoppingCartItemWidget()
{
    delete ui;
}

void ShoppingCartItemWidget::setCode(const QString &code)
{
    ui->code->setText("  " + code);
}

void ShoppingCartItemWidget::setName(const QString &name)
{
    ui->name_2->setText("  " + name);
}

void ShoppingCartItemWidget::setPrice(const QString &price)
{
    ui->price->setText("  " + price + "تومان");
}

int ShoppingCartItemWidget::quantity() const
{
    return ui->spinBox->value();
}

void ShoppingCartItemWidget::onRemoveClicked()
{
    emit removeClicked();
}
=======

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

>>>>>>> 3c46e57084e3e05929fdd3eeca940a40a1568c58
