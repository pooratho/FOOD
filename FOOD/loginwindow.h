#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include "database.h"
namespace Ui {
class Form;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void on_comboBox_activated(int index);

    void on_pushButton_clicked();

private:
    Ui::Form *ui;
};

#endif // LOGINWINDOW_H
