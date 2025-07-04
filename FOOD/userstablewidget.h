#ifndef USERSTABLEWIDGET_H
#define USERSTABLEWIDGET_H

#include "clientsocketmanager.h"
#include <QWidget>

namespace Ui {
class usertablewidget;          // این همون کلاسی است که uic از .ui می‌سازد
}

class UserTableWidget : public QWidget
{
    Q_OBJECT

public:
    UserTableWidget(ClientSocketManager* socketManager, QWidget *parent = nullptr);

    ~UserTableWidget();

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::usertablewidget *ui;    // اشاره‌گر به شیء تولیدی uic
    ClientSocketManager* clientSocket = nullptr;
};

#endif // USERSTABLEWIDGET_H
