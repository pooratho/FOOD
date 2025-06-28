#include "servermanager.h"
#include <QSqlQuery>
#include <QSqlError>

ServerManager::ServerManager(QObject *parent)
    : QObject(parent)
{
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &ServerManager::onNewConnection);
}

bool ServerManager::startServer(int port)
{
    if (!server->listen(QHostAddress::Any, port)) {
        emit logMessage("❌ سرور راه‌اندازی نشد.");
        return false;
    } else {
        emit logMessage("✅ سرور روی پورت " + QString::number(port) + " اجرا شد.");
        return true;
    }
}

void ServerManager::connectToDatabase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("users.db");

    if (!db.open()) {
        emit logMessage("❌ اتصال به دیتابیس ناموفق بود.");
    } else {
        emit logMessage("✅ اتصال به دیتابیس برقرار شد.");
    }
}

void ServerManager::onNewConnection()
{
    QTcpSocket *client = server->nextPendingConnection();
    clients.append(client);

    connect(client, &QTcpSocket::readyRead, this, &ServerManager::onReadyRead);
    connect(client, &QTcpSocket::disconnected, this, &ServerManager::onDisconnected);

    emit logMessage("🔌 کلاینت جدید وصل شد.");
}

void ServerManager::onReadyRead()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    QString msg = QString::fromUtf8(client->readAll());

    emit logMessage("📨 پیام دریافتی: " + msg);
    processMessage(client, msg);
}

void ServerManager::onDisconnected()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    clients.removeAll(client);
    client->deleteLater();

    emit logMessage("❌ کلاینت قطع شد.");
}

void ServerManager::processMessage(QTcpSocket *client, const QString &msg)
{
    if (msg.startsWith("LOGIN:")) {
        QStringList parts = msg.split(":");
        if (parts.size() != 3) {
            client->write("LOGIN_FAIL:فرمت اشتباه");
            return;
        }

        QString username = parts[1];
        QString password = parts[2];

        QSqlQuery query;
        query.prepare("SELECT * FROM users WHERE username = ? AND password = ?");
        query.addBindValue(username);
        query.addBindValue(password);

        if (query.exec() && query.next()) {
            QString role = query.value("role").toString();
            client->write(("LOGIN_OK:" + role).toUtf8());
        } else {
            client->write("LOGIN_FAIL:نام کاربری یا رمز اشتباه است");
        }
    }
}
