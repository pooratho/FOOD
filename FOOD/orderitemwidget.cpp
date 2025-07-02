// orderitemwidget.cpp
#include "orderitemwidget.h"

OrderItemWidget::OrderItemWidget(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);
}

OrderItemWidget::~OrderItemWidget()
{
}

// متدهای تنظیم نام و کد
void OrderItemWidget::setName(const QString& name)
{
    ui.name->setText(name);
}

void OrderItemWidget::setCode(const QString& code)
{
    ui.code->setText(code);
}
