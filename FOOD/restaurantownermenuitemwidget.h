// restaurantownermenuitemwidget.h

#ifndef RESTAURANTOWNERMENUITEMWIDGET_H
#define RESTAURANTOWNERMENUITEMWIDGET_H

#include <QWidget>
#include "food.h"  // 👈 یادت نره اینو اضافه کنی

namespace Ui {
class restaurantownermenuitemwidget;
}

class RestaurantOwnerMenuItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RestaurantOwnerMenuItemWidget(QWidget *parent = nullptr);
    ~RestaurantOwnerMenuItemWidget();

  void setItemInfo(const Food& food);
    void setFood(const Food& food);     //  setter
    Food getFood() const;               //  getter

signals:
    void removeRequested();

private slots:
    void on_pushButton_clicked();

private:
    Ui::restaurantownermenuitemwidget *ui;
    Food currentFood;
};

#endif // RESTAURANTOWNERMENUITEMWIDGET_H
