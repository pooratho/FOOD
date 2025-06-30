#ifndef RESTAURANTMENUITEMWIDGET_H
#define RESTAURANTMENUITEMWIDGET_H

#include <QWidget>

namespace Ui {
class restaurantmenuitemwidget;
}

class RestaurantMenuItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RestaurantMenuItemWidget(QWidget *parent = nullptr);
    ~RestaurantMenuItemWidget();

    void setName(const QString& name);
    void setDescription(const QString& desc);
    void setPrice(const QString& price);
    bool isSelected() const;

private:
    Ui::restaurantmenuitemwidget *ui;
};

#endif // RESTAURANTMENUITEMWIDGET_H
