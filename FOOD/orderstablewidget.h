#ifndef ORDERSTABLEWIDGET_H
#define ORDERSTABLEWIDGET_H

#include <QWidget>

namespace Ui {
class orderstablewidget;
}

class OrdersTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OrdersTableWidget(QWidget *parent = nullptr);
    ~OrdersTableWidget();

private:
    Ui::orderstablewidget *ui;
};

#endif // ORDERSTABLEWIDGET_H
