#ifndef RESTAURANTOWNERSIGNUPWINDOW_H
#define RESTAURANTOWNERSIGNUPWINDOW_H

#include "database.h"
#include <QWidget>
#include <QMessageBox>
namespace Ui {
class RestaurantOwnerSignUpWindow;
}

class RestaurantOwnerSignUpWindow : public QWidget
{
    Q_OBJECT

public:
    explicit RestaurantOwnerSignUpWindow(QWidget *parent = nullptr);
    ~RestaurantOwnerSignUpWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::RestaurantOwnerSignUpWindow *ui;
    DatabaseManager dbManager;
};


#endif // RESTAURANTOWNERSIGNUPWINDOW_H
