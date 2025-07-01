#ifndef RESTAURANTOWNERMAINPAGE_H
#define RESTAURANTOWNERMAINPAGE_H

#include <QWidget>
#include <QMap>   // ⬅️ لازم برای نگه‌داری منو دسته‌بندی‌شده
#include "restaurantowner.h"
#include "food.h"
#include "menuitem.h"
#include "clientsocketmanager.h"

namespace Ui {
class restaurantOwnerMainPage;
}

class RestaurantOwnerMainPage : public QWidget
{
    Q_OBJECT

public:
    explicit RestaurantOwnerMainPage(RestaurantOwner* owner, QWidget *parent = nullptr);
    ~RestaurantOwnerMainPage();

private slots:
    void on_pushButton_clicked();              // افزودن غذا
    void handleServerMessage(const QString &msg);  // پاسخ سرور

private:
    Ui::restaurantOwnerMainPage *ui;
    RestaurantOwner* currentOwner;

    // به جای فقط یک MenuItem، همه‌ی دسته‌ها رو نگه داریم:
    QMap<QString, MenuItem> menuByCategory;   // ⬅ این نگه‌دارنده منوی دسته‌بندی شده‌ست

    ClientSocketManager *clientSocket;
};

#endif // RESTAURANTOWNERMAINPAGE_H
