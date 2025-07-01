#ifndef SHOPPINGCARTITEMWIDGET_H
#define SHOPPINGCARTITEMWIDGET_H

#include <QWidget>

namespace Ui {
class shoppingcartitemwidget;
}

class ShoppingCartItemWidget : public QWidget
{
    Q_OBJECT

public:
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
};

#endif // SHOPPINGCARTITEMWIDGET_H
