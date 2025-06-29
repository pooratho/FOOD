#ifndef RESTAURANTOWNERSIGNUP_H
#define RESTAURANTOWNERSIGNUP_H

#include <QWidget>
#include <QMessageBox>
#include "clientsocketmanager.h"  // کلاس ارتباط با سرور
#include <QMap>

#include "restaurantownermainpage.h"

class LoginWindow;

namespace Ui {
class RestaurantOwnerSignUp;
}

class RestaurantOwnerSignUp : public QWidget
{
    Q_OBJECT

public:
    explicit RestaurantOwnerSignUp(LoginWindow *loginWin, QWidget *parent = nullptr);
    ~RestaurantOwnerSignUp();

private slots:
    void on_pushButton_clicked();

    void on_comboBoxProvince_currentTextChanged(const QString &province);

    void on_comboBoxCity_currentTextChanged(const QString &city);

private:
    Ui::RestaurantOwnerSignUp *ui;
    ClientSocketManager *clientSocket;  // اشاره‌گر به سوکت مدیریتی
    QMap<QString, QStringList> provinceCitiesMap;
    LoginWindow *loginWindow;
};

#endif // RESTAURANTOWNERSIGNUPWINDOW_H

