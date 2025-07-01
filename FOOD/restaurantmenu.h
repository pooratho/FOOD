#ifndef RESTAURANTMENU_H
#define RESTAURANTMENU_H

#include <QWidget>
#include "clientsocketmanager.h"
#include "qlistwidget.h"

namespace Ui {
class restaurantmenu;
}

class restaurantmenu : public QWidget
{
    Q_OBJECT

public:
    explicit restaurantmenu(const QString& restaurantName, QWidget *parent = nullptr);
    ~restaurantmenu();

private:
    Ui::restaurantmenu *ui;
    ClientSocketManager* clientSocket;
    QString restaurantName;  // ✅ بهتر از currentRestaurantName
    void clearListWidgetCompletely(QListWidget* listWidget);

private slots:
    void handleServerMessage(const QString& msg);
    void populateMenuItems(const QStringList& items);
    void on_pushButton_clicked();

signals:
    void requestShowCart();  // این باید اینجا باشه

};

#endif // RESTAURANTMENU_H
