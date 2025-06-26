#ifndef CUSTOMERSIGNUPWINDOW_H
#define CUSTOMERSIGNUPWINDOW_H

#include <QWidget>

namespace Ui {
class Form;  // همان کلاس تولیدشده توسط uic
}

class CustomerSignUpWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CustomerSignUpWindow(QWidget *parent = nullptr);
    ~CustomerSignUpWindow();

private:
    Ui::Form *ui;
};

#endif // CUSTOMERSIGNUPWINDOW_H
