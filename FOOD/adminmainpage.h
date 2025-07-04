#ifndef ADMINMAINPAGE_H
#define ADMINMAINPAGE_H

#include <QWidget>

#include "clientsocketmanager.h"
#include "userstablewidget.h"
#include "orderstablewidget.h"

#include <QMap>

QT_BEGIN_NAMESPACE
namespace Ui { class AdminMainPage; }
QT_END_NAMESPACE

class RestaurantTableWidget;

class AdminMainPage : public QWidget
{
    Q_OBJECT

private slots:
    void handleServerMessage(const QString &msg);

public:
    explicit AdminMainPage(QWidget *parent = nullptr);
    ~AdminMainPage();

private:
    Ui::AdminMainPage *ui;
    ClientSocketManager* clientSocket;
    RestaurantTableWidget *tableWin = nullptr;
    UserTableWidget* userTableWin = nullptr;
    OrdersTableWidget *ordersTableWin = nullptr;
    QMap<QString, QString> userPhoneToName;
    QString exportDestination;

};
#endif // ADMINMAINPAGE_H
