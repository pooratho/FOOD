#ifndef RESTAURANTTABLEWIDGET_H
#define RESTAURANTTABLEWIDGET_H

#include "clientsocketmanager.h"
#include <QWidget>

namespace Ui {
class restauranttablewidget;  // ✅ مطابق فایل UI
}

class RestaurantTableWidget : public QWidget
{
    Q_OBJECT

public:
   explicit RestaurantTableWidget(ClientSocketManager* socketManager, QWidget *parent = nullptr);
    ~RestaurantTableWidget();

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::restauranttablewidget *ui;  // ✅ مطمئن شو این دقیقاً همین نام باشه
     ClientSocketManager* clientSocket = nullptr;
};

#endif // RESTAURANTTABLEWIDGET_H
