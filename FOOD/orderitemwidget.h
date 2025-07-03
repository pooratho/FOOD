// orderitemwidget.h
#ifndef ORDERITEMWIDGET_H
#define ORDERITEMWIDGET_H

#include <QDialog>

namespace Ui {
class orderitemwidget;
}

class orderitemwidget : public QDialog
{
    Q_OBJECT

public:
    explicit orderitemwidget(QWidget *parent = nullptr);
    ~orderitemwidget();

    void setOrderId(int id) ;
    int getOrderId() const ;

public slots:
    void setRestaurantName(const QString& name);
    void setFoodListText(const QString& text);
    //void setTotalPrice(double price);
    void setStatus(const QString& status);


private:
    Ui::orderitemwidget *ui;
     int orderId;
};

#endif // ORDERITEMWIDGET_H
