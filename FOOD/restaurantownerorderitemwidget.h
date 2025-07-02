#ifndef RESTAURANTOWNERORDERITEMWIDGET_H
#define RESTAURANTOWNERORDERITEMWIDGET_H

// restaurantownerorderitemwidget.h
#include <QWidget>
#include "ui_restaurantownerorderitemwidget.h"    // ← هیدر تولیدى uic

class RestaurantOwnerOrderItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RestaurantOwnerOrderItemWidget(QWidget *parent = nullptr);

    // متدهاى کمکی براى پر کردن ویجت
    void setRestaurantName(const QString &name);
    void setCustomerName(const QString &name);
    void setFoodText(const QString &foods);
    void setStatus(const QString &status);

private slots:

private:
    Ui::restaurantownerorderitemwidget ui;   // شیِ UI (به‌صورت value یا pointer)
};


#endif // RESTAURANTOWNERORDERITEMWIDGET_H
