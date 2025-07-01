#ifndef CUSTOMERMAINPAGE_H
#define CUSTOMERMAINPAGE_H

#include <QWidget>
#include <QMap>
#include <QStringList>
#include <QTableWidgetItem>

#include "customer.h"
#include "clientsocketmanager.h"  // برای اتصال به سرور

namespace Ui {
class CustomerMainPage;
}

class CustomerMainPage : public QWidget
{
    Q_OBJECT

public:
    explicit CustomerMainPage(Customer* customer, QWidget *parent = nullptr);
    ~CustomerMainPage();

private slots:
    void on_comboBox_2_currentTextChanged(const QString &province);
    void on_comboBox_3_currentTextChanged(const QString &city);

    void handleServerMessage(const QString &msg); //  اسلات برای گرفتن پیام‌های سرور

private:
    Ui::CustomerMainPage *ui;
    QMap<QString, QStringList> provinceCitiesMap;
    Customer* customer;

    ClientSocketManager* clientSocket;
};

#endif // CUSTOMERMAINPAGE_H
