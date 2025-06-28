#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QSqlDatabase>

class ServerManager : public QObject
{
    Q_OBJECT
public:
    explicit ServerManager(QObject *parent = nullptr);
    bool startServer(int port);
    void connectToDatabase();

signals:
    void logMessage(const QString &msg);

private slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();

private:
    QTcpServer *server;
    QList<QTcpSocket*> clients;
    QSqlDatabase db;

    void processMessage(QTcpSocket *client, const QString &msg);
};

#endif // SERVERMANAGER_H
