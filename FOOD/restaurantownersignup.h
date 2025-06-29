#ifndef RESTAURANTOWNERSIGNUP_H
#define RESTAURANTOWNERSIGNUP_H

#include "database.h"
#include <QWidget>
#include <QMessageBox>
namespace Ui {
class RestaurantOwnerSignUp;
}

class RestaurantOwnerSignUp : public QWidget
{
    Q_OBJECT

public:
    explicit RestaurantOwnerSignUp(QWidget *parent = nullptr);
    ~RestaurantOwnerSignUp();

private slots:
    void on_pushButton_clicked();

private:
    Ui::RestaurantOwnerSignUp *ui;
    DatabaseManager dbManager;
};


#endif // RESTAURANTOWNERSIGNUP_H
