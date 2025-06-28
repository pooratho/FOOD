#ifndef CUSTOMERSIGNUP_H
#define CUSTOMERSIGNUP_H

#include <QDialog>
#include <QMessageBox>
#include "clientsocketmanager.h"  // مدیریت ارتباط با سرور

namespace Ui {
class customersignup;
}

class customersignup : public QDialog
{
    Q_OBJECT

public:
    explicit customersignup(QWidget *parent = nullptr);
    ~customersignup();

private slots:
    void on_pushButton_clicked();

private:
    Ui::customersignup *ui;
    ClientSocketManager *clientSocket;
};

#endif // CUSTOMERSIGNUP_H
