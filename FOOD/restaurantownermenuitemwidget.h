#ifndef RESTAURANTOWNERMENUITEMWIDGET_H
#define RESTAURANTOWNERMENUITEMWIDGET_H

#include <QWidget>

namespace Ui {
class restaurantownermenuitemwidget;
}

class RestaurantOwnerMenuItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RestaurantOwnerMenuItemWidget(QWidget *parent = nullptr);
    ~RestaurantOwnerMenuItemWidget();

    void setItemInfo(const QString &name, const QString &description, const QString &priceText);

signals:
    void removeRequested();  // سیگنال برای وقتی که دکمه حذف زده میشه

private slots:

    void on_pushButton_clicked();

private:
    Ui::restaurantownermenuitemwidget *ui;
};

#endif // RESTAURANTOWNERMENUITEMWIDGET_H
