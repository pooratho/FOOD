#ifndef RESTAURANTOWNERSIGNUPWINDOW_H
#define RESTAURANTOWNERSIGNUPWINDOW_H

#include <QWidget>

namespace Ui {
class Form;
}

class RestaurantOwnerSignUpWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RestaurantOwnerSignUpWindow(QWidget *parent = nullptr);
    ~RestaurantOwnerSignUpWindow();

private:
    Ui::Form *ui;
};

#endif // RESTAURANTOWNERSIGNUPWINDOW_H
