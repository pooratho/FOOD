#include "shoppingcartitemwidget.h"
#include "ui_shoppingcartitemwidget.h"

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
