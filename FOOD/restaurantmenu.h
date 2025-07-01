#ifndef RESTAURANTMENU_H
#define RESTAURANTMENU_H

#include <QWidget>
#include "clientsocketmanager.h"  // اضافه کن

namespace Ui {
class restaurantmenu;
}

class restaurantmenu : public QWidget
{
    Q_OBJECT

public:
    explicit restaurantmenu(const QString& restaurantName, QWidget *parent = nullptr); // ✔️ این تغییر
    ~restaurantmenu();

private:
    Ui::restaurantmenu *ui;
    ClientSocketManager* clientSocket;  // ✔️ اضافه کردن سوکت
    QString currentRestaurantName;      // ✔️ ذخیره نام رستوران

private slots:
    void handleServerMessage(const QString& msg); // ✔️ دریافت پیام سرور
};

#endif // RESTAURANTMENU_H
