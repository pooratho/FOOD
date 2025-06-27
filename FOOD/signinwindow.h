#ifndef SIGNINWINDOW_H
#define SIGNINWINDOW_H

#include <QWidget>

namespace Ui {
class Form;
}

class SignInWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SignInWindow(QWidget *parent = nullptr);
    ~SignInWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Form *ui;
};

#endif // SIGNINWINDOW_H
