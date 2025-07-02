// orderitemwidget.h
#ifndef ORDERITEMWIDGET_H
#define ORDERITEMWIDGET_H

#include <QWidget>
#include "ui_orderitemwidget.h"

class OrderItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OrderItemWidget(QWidget *parent = nullptr);
    ~OrderItemWidget();

    // می‌توانید متدهایی برای تنظیم داده‌ها اضافه کنید
    void setName(const QString& name);
    void setCode(const QString& code);

private:
    Ui::orderitemwidget ui;
};

#endif // ORDERITEMWIDGET_H
