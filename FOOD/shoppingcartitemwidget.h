#ifndef SHOPPINGCARTITEMWIDGET_H
#define SHOPPINGCARTITEMWIDGET_H

#include <QWidget>
#include "cartitem.h"

namespace Ui {
<<<<<<< HEAD
class shoppingcartitemwidget;
=======
class ShoppingCartItemWidget;
>>>>>>> 3c46e57084e3e05929fdd3eeca940a40a1568c58
}

class ShoppingCartItemWidget : public QWidget
{
    Q_OBJECT

public:
<<<<<<< HEAD
    explicit ShoppingCartItemWidget(QWidget *parent = nullptr);
    ~ShoppingCartItemWidget();

    void setCode(const QString &code);
    void setName(const QString &name);
    void setPrice(const QString &price);
    int quantity() const;

signals:
    void removeClicked();

private slots:
    void onRemoveClicked();

private:
    Ui::shoppingcartitemwidget *ui;
=======
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
>>>>>>> 3c46e57084e3e05929fdd3eeca940a40a1568c58
};

#endif // SHOPPINGCARTITEMWIDGET_H
