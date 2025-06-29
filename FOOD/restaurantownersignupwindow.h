#ifndef RESTAURANTOWNERSIGNUPWINDOW_H
#define RESTAURANTOWNERSIGNUPWINDOW_H

#include <QWidget>
#include <QMessageBox>
#include "clientsocketmanager.h"

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
    ClientSocketManager *clientSocket;  // اشاره‌گر به سوکت مدیریتی
};

#endif // RESTAURANTOWNERSIGNUPWINDOW_H
