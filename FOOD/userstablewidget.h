#ifndef USERSTABLEWIDGET_H
#define USERSTABLEWIDGET_H

#include <QWidget>

namespace Ui {
class usertablewidget;          // این همون کلاسی است که uic از .ui می‌سازد
}

class UserTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserTableWidget(QWidget *parent = nullptr);
    ~UserTableWidget();

private:
    Ui::usertablewidget *ui;    // اشاره‌گر به شیء تولیدی uic
};

#endif // USERSTABLEWIDGET_H
