#ifndef CLIENTSOCKETMANAGER_H
#define CLIENTSOCKETMANAGER_H

#include <QTcpSocket>
#include <QObject>

class ClientSocketManager : public QObject
{
    Q_OBJECT

public:
    explicit ClientSocketManager(QObject *parent = nullptr);

    void connectToServer(const QString &host, quint16 port);
    void sendMessage(const QString &msg);

signals:
    void messageReceived(const QString &msg);
    void connected();
    void disconnected();
    void errorOccurred(const QString &error);

private slots:
    void onReadyRead();
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket *socket;
};

#endif // CLIENTSOCKETMANAGER_H
