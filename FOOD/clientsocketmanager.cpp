#include "clientsocketmanager.h"
#include <QDebug>

ClientSocketManager::ClientSocketManager(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::readyRead, this, &ClientSocketManager::onReadyRead);
    connect(socket, &QTcpSocket::connected, this, &ClientSocketManager::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &ClientSocketManager::onDisconnected);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, &ClientSocketManager::onError);
}

void ClientSocketManager::connectToServer(const QString &host, quint16 port)
{
    socket->connectToHost(host, port);
}

void ClientSocketManager::sendMessage(const QString &msg)
{

    if (socket->state() == QTcpSocket::ConnectedState) {
        socket->write((msg + "\n").toUtf8());  // اضافه کردن \n برای تمایز بین پیام‌ها
    } else {
        emit errorOccurred("اتصال به سرور برقرار نیست.");
    }
}

void ClientSocketManager::onReadyRead()
{
    static QString buffer;

    buffer += QString::fromUtf8(socket->readAll());

    while (buffer.contains("\n")) {
        int idx = buffer.indexOf("\n");
        QString msg = buffer.left(idx).trimmed();
        buffer = buffer.mid(idx + 1);
        emit messageReceived(msg);
    }
}


void ClientSocketManager::onConnected()
{
    emit connected();
}

void ClientSocketManager::onDisconnected()
{
    emit disconnected();
}

void ClientSocketManager::onError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError)
    emit errorOccurred(socket->errorString());
}
