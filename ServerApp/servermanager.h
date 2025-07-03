#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include "database.h"

class ServerManager : public QObject
{
    Q_OBJECT
public:
    explicit ServerManager(QObject *parent = nullptr);
    ~ServerManager();

    void connectToDatabase();
    void startServer(quint16 port);

signals:
    void logMessage(const QString &msg);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();

private:
    QTcpServer *server;
    QList<QTcpSocket*> clients;
    DatabaseManager dbManager;
    QMap<QTcpSocket*, int> restaurantSocketMap;
    QTcpSocket* getSocketByCustomerId(int customerId);
    void processMessage(QTcpSocket *sender, const QString &msg);
    QMap<int, QTcpSocket*> customerIdToSocketMap;

};

#endif // SERVERMANAGER_H
