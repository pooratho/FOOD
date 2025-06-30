#ifndef RESTAURANTOWNERSIGNUP_H
#define RESTAURANTOWNERSIGNUP_H

#include <QWidget>
#include <QMessageBox>
#include "clientsocketmanager.h"
#include <QMap>
#include "restaurantownermainpage.h"
#include "restaurantowner.h"

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
    ClientSocketManager *clientSocket;
    QMap<QString, QStringList> provinceCitiesMap;
    LoginWindow *loginWindow;

    // اضافه کردن متغیر کش برای اطلاعات ثبت‌نام
    QString cachedRestaurantName;
    QString cachedOwnerFirstName;
    QString cachedOwnerLastName;
    QString cachedPhone;
    QString cachedProvince;
    QString cachedCity;
    QString cachedPassword;
};

#endif // RESTAURANTOWNERSIGNUP_H
