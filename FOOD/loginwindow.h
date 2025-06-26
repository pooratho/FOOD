#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>

namespace Ui {
class Form;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private:
    Ui::Form *ui;
};

#endif // LOGINWINDOW_H
