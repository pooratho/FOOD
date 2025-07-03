#ifndef ORDERITEMWIDGET_H
#define ORDERITEMWIDGET_H

#include <QDialog>

namespace Ui {
class orderitemwidgett;
}

class orderitemwidgett : public QDialog
{
    Q_OBJECT

public:
    explicit orderitemwidgett(QWidget *parent = nullptr);
    ~orderitemwidgett();
public slots:
    void setRestaurantName(const QString& name);
    void setFoodListText(const QString& text);
    //void setTotalPrice(double price);
    void setStatus(const QString& status);


private:
    Ui::orderitemwidgett *ui;
};

#endif // ORDERITEMWIDGET_H
