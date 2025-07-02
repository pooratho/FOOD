#ifndef SHOPPINGCARTITEMWIDGET_H
#define SHOPPINGCARTITEMWIDGET_H

#include <QWidget>
#include "cartitem.h"

namespace Ui {
class ShoppingCartItemWidget;
}

class ShoppingCartItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ShoppingCartItemWidget(const CartItem& item, QWidget *parent = nullptr);
    ~ShoppingCartItemWidget();

    CartItem getCartItem() const;

signals:
    void quantityChanged(int newQuantity);
    void removeClicked(ShoppingCartItemWidget* self);

private slots:
    void on_spinBoxQuantity_valueChanged(int value);

    void on_pushButton_clicked();

private:
    Ui::ShoppingCartItemWidget *ui;
    CartItem cartItem;

    void updateUi();
};

#endif // SHOPPINGCARTITEMWIDGET_H
