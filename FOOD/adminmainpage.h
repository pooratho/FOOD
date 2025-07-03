#ifndef ADMINMAINPAGE_H
#define ADMINMAINPAGE_H

#include <QWidget>

#include "clientsocketmanager.h"


QT_BEGIN_NAMESPACE
namespace Ui { class AdminMainPage; }
QT_END_NAMESPACE

// ↙ اگر RestaurantTableWidget را forward declare کنید نیازی به include داخل هدر نیست:
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
};
#endif // ADMINMAINPAGE_H
