#ifndef RESTAURANTOWNERSIGNUP_H
#define RESTAURANTOWNERSIGNUP_H

#include <QWidget>
#include <QMessageBox>
#include "clientsocketmanager.h"  // کلاس ارتباط با سرور

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
    ClientSocketManager *clientSocket;  // اشاره‌گر به سوکت مدیریتی
};

#endif // RESTAURANTOWNERSIGNUPWINDOW_H

