#ifndef ADMINMAINPAGE_H
#define ADMINMAINPAGE_H

#include <QWidget>

#include "clientsocketmanager.h"
#include "userstablewidget.h"
#include "orderstablewidget.h"

#include <QMap>
#include <QFile>

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
    QFile     receivingFile;              // فایلی که روی دیسک می‌نویسیم
    qint64    expectedBytes{0};           // حجمی که سرور اعلام می‌کند
    qint64    receivedBytes{0};           // حجمِ دریافت‌شده تا این لحظه
    enum ReceivingState { None, ReceivingDb } receivingState{None};

    QString pendingSavePath;          // جایی که کاربر می‌خواهد فایل را ذخیره کند
    bool    waitingForDump {false};   // آیا در حال انتظار برای دیتاست؟

};
#endif // ADMINMAINPAGE_H
