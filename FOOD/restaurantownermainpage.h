#ifndef RESTAURANTOWNERMAINPAGE_H
#define RESTAURANTOWNERMAINPAGE_H

#include <QWidget>
#include <QMap>   // ⬅ لازم برای نگه‌داری منو دسته‌بندی‌شده
#include "qlistwidget.h"
#include "restaurantowner.h"
#include "food.h"
#include "menuitem.h"
#include "clientsocketmanager.h"
#include "restaurantownerorderitemwidget.h"
#include <QLabel>


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
    void on_tabWidget_currentChanged(int index);

    void on_pushButton_2_clicked();

private:
    Ui::restaurantOwnerMainPage *ui;
    RestaurantOwner* currentOwner;

    QLabel* notificationLabel = nullptr;
QSet<int> shownOrderIds;
    int lastOrderId = 0;  // یا QString اگر آیدی رشته است
    int currentOrderCount = 0;

    // به جای فقط یک MenuItem، همه‌ی دسته‌ها رو نگه داریم:
    QMap<QString, MenuItem> menuByCategory;   //  این نگه‌دارنده منوی دسته‌بندی شده‌ست
QVector<QPair<QListWidget*, QWidget*>> pendingDeletions;
    ClientSocketManager *clientSocket;
void populateMenuItems(const QStringList& items);
    void clearListWidgetCompletely(QListWidget* listWidget);
void clearOrderListWidget();
    void showNewOrderNotification(const QString& msg);
};

#endif // RESTAURANTOWNERMAINPAGE_H
