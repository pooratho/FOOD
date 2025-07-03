#ifndef RESTAURANTOWNERORDERITEMWIDGET_H
#define RESTAURANTOWNERORDERITEMWIDGET_H

#include <QWidget>

namespace Ui {
class restaurantownerorderitemwidget;
}

class RestaurantOwnerOrderItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RestaurantOwnerOrderItemWidget(QWidget *parent = nullptr);
    ~RestaurantOwnerOrderItemWidget();

    void setFoodText(const QString &foods);
    void setCustomerPhone(const QString &phone);
    void setStatus(const QString &status);  // مقداردهی کامبوباکس به صورت اولیه

signals:
    void updateStatusRequested(const QString &newStatus);  // سیگنال وقتی وضعیت تغییر کرد

private slots:
    void onStatusChanged(int index);  // اسلات داخلی برای تغییر وضعیت

private:
    Ui::restaurantownerorderitemwidget *ui;

    void setupStatusComboBox();  // مقداردهی آیتم های کامبوباکس
};

#endif // RESTAURANTOWNERORDERITEMWIDGET_H
