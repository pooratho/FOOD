#ifndef RESTAURANTTABLEWIDGET_H
#define RESTAURANTTABLEWIDGET_H

#include <QWidget>

namespace Ui {
class restauranttablewidget;  // ✅ مطابق فایل UI
}

class RestaurantTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RestaurantTableWidget(QWidget *parent = nullptr);
    ~RestaurantTableWidget();

private:
    Ui::restauranttablewidget *ui;  // ✅ مطمئن شو این دقیقاً همین نام باشه
};

#endif // RESTAURANTTABLEWIDGET_H
