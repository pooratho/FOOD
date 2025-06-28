#include "servermanager.h"
#include <QDebug>

ServerManager::ServerManager(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &ServerManager::onNewConnection);
}

ServerManager::~ServerManager()
{
    // دیتابیس خود dbManager مدیریت می‌کنه
}

void ServerManager::connectToDatabase()
{
    if (dbManager.connectToDatabase()) {
        emit logMessage("✅ اتصال به دیتابیس برقرار شد.");
        dbManager.createTables();
    } else {
        emit logMessage("❌ اتصال به دیتابیس ناموفق بود.");
    }
}

void ServerManager::startServer(quint16 port)
{
    if (!server->listen(QHostAddress::Any, port)) {
        emit logMessage("❌ سرور راه‌اندازی نشد: " + server->errorString());
    } else {
        emit logMessage("✅ سرور روی پورت " + QString::number(port) + " اجرا شد.");
    }
}

void ServerManager::onNewConnection()
{
    QTcpSocket *client = server->nextPendingConnection();
    clients.append(client);

    connect(client, &QTcpSocket::readyRead, this, &ServerManager::onReadyRead);
    connect(client, &QTcpSocket::disconnected, this, &ServerManager::onDisconnected);

    emit logMessage("🔌 کلاینت جدید متصل شد: " + client->peerAddress().toString());
}

void ServerManager::onReadyRead()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (!client)
        return;

    QByteArray data = client->readAll();
    QString msg = QString::fromUtf8(data).trimmed();

    emit logMessage("📨 پیام دریافتی: " + msg);

    processMessage(client, msg);
}

void ServerManager::onDisconnected()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (!client)
        return;

    clients.removeAll(client);
    emit logMessage("❌ کلاینت قطع شد: " + client->peerAddress().toString());
    client->deleteLater();
}

void ServerManager::processMessage(QTcpSocket *sender, const QString &msg)
{
    if (msg.startsWith("LOGIN:")) {
        QStringList parts = msg.split(":");
        if(parts.size() != 4) {
            sender->write("LOGIN_FAIL:فرمت اشتباه\n");
            return;
        }

        QString firstName = parts[1].trimmed();
        QString lastName = parts[2].trimmed();
        QString password = parts[3].trimmed();

        auto role = dbManager.checkUserLogin(firstName, lastName, password);
        if (role != DatabaseManager::UserRole::None) {
            QString roleStr;
            switch(role) {
            case DatabaseManager::UserRole::Customer: roleStr = "Customer"; break;
            case DatabaseManager::UserRole::Restaurant: roleStr = "Restaurant"; break;
            case DatabaseManager::UserRole::Admin: roleStr = "Admin"; break;
            default: roleStr = "None"; break;
            }
            sender->write(("LOGIN_OK:" + roleStr + "\n").toUtf8());
            emit logMessage("✅ ورود موفق: " + firstName + " " + lastName);
        } else {
            sender->write("LOGIN_FAIL:اطلاعات نادرست\n");
            emit logMessage("⚠️ ورود ناموفق برای: " + firstName + " " + lastName);
        }
    }

    else if (msg.startsWith("SIGNUP:")) {
        QStringList parts = msg.split(":");
        if (parts.size() < 2) {
            sender->write("SIGNUP_FAIL:فرمت اشتباه\n");
            return;
        }

        QString role = parts[1].trimmed().toLower();

        if (role == "customer" && parts.size() == 6) {
            QString firstName = parts[2].trimmed();
            QString lastName = parts[3].trimmed();
            QString phone = parts[4].trimmed();
            QString password = parts[5].trimmed();

            bool ok = dbManager.insertCustomer(firstName, lastName, phone, password);
            if (ok) {
                sender->write("SIGNUP_OK:Customer\n");
                emit logMessage("✅ ثبت مشتری جدید: " + firstName + " " + lastName);
            } else {
                sender->write("SIGNUP_FAIL:Customer\n");
                emit logMessage("❌ ثبت مشتری ناموفق");
            }
        }

        else if (role == "restaurant" && parts.size() == 9) {
            QString restName      = parts[2].trimmed();
            QString ownerFirst    = parts[3].trimmed();
            QString ownerLast     = parts[4].trimmed();
            QString phone         = parts[5].trimmed();
            QString province      = parts[6].trimmed();
            QString city          = parts[7].trimmed();
            QString password      = parts[8].trimmed();

            bool ok = dbManager.insertRestaurant(restName, ownerFirst, ownerLast, phone, province, city, password);
            if (ok) {
                sender->write("SIGNUP_OK:Restaurant\n");
                emit logMessage("✅ ثبت رستوران جدید: " + restName);
            } else {
                sender->write("SIGNUP_FAIL:Restaurant\n");
                emit logMessage("❌ ثبت رستوران ناموفق");
            }
        }

        else {
            sender->write("SIGNUP_FAIL:فرمت نامعتبر\n");
        }
    }

    else if (msg.startsWith("SIGNUP_CUSTOMER:")) {
        QStringList parts = msg.split(":");
        if (parts.size() < 2) {
            sender->write("SIGNUP_FAIL:فرمت اشتباه\n");
            return;
        }

        QString role = parts[1].trimmed().toLower();

        if (role == "customer" && parts.size() == 6) {
            QString firstName = parts[2].trimmed();
            QString lastName = parts[3].trimmed();
            QString phone = parts[4].trimmed();
            QString password = parts[5].trimmed();

            bool ok = dbManager.insertCustomer(firstName, lastName, phone, password);
            if (ok) {
                sender->write("SIGNUP_OK:Customer\n");
                emit logMessage("✅ ثبت مشتری جدید: " + firstName + " " + lastName);
            } else {
                sender->write("SIGNUP_FAIL:Customer\n");
                emit logMessage("❌ ثبت مشتری ناموفق");
            }
        }

        else {
            sender->write("SIGNUP_FAIL:فرمت نامعتبر\n");
        }
    }

    else {
        sender->write("ERROR:فرمان ناشناخته\n");
    }
}

// void ServerManager::processMessage(QTcpSocket *sender, const QString &msg)
// {
//     if (msg.startsWith("LOGIN:")) {
//         QStringList parts = msg.split(":");
//         if(parts.size() != 4) {
//             sender->write("LOGIN_FAIL:فرمت اشتباه\n");
//             return;
//         }

//         QString firstName = parts[1].trimmed();
//         QString lastName = parts[2].trimmed();
//         QString password = parts[3].trimmed();

//         auto role = dbManager.checkUserLogin(firstName, lastName, password);
//         if (role != DatabaseManager::UserRole::None) {
//             QString roleStr;
//             switch(role) {
//             case DatabaseManager::UserRole::Customer: roleStr = "Customer"; break;
//             case DatabaseManager::UserRole::Restaurant: roleStr = "Restaurant"; break;
//             case DatabaseManager::UserRole::Admin: roleStr = "Admin"; break;
//             default: roleStr = "None"; break;
//             }
//             sender->write(("LOGIN_OK:" + roleStr + "\n").toUtf8());
//             emit logMessage("✅ ورود موفق: " + firstName + " " + lastName);
//         } else {
//             sender->write("LOGIN_FAIL:اطلاعات نادرست\n");
//             emit logMessage("⚠️ ورود ناموفق برای: " + firstName + " " + lastName);
//         }
//     }
//     else if (msg.startsWith("SIGNUP_RESTAURANT:")) {
//         QStringList parts = msg.split(":");
//         if(parts.size() != 7) {
//             sender->write("SIGNUP_FAIL:فرمت اشتباه\n");
//             return;
//         }
//         QString restName = parts[1].trimmed();
//         QString ownerFirst = parts[2].trimmed();
//         QString ownerLast = parts[3].trimmed();
//         QString phone = parts[4].trimmed();
//         QString province = parts[5].trimmed();
//         QString city = parts[6].trimmed();
//         QString password; // باید از جایی بگیری (مثلاً اضافه کن به پیام یا ثابت بذار)

//         // مثلا فرض کن رمز ثابت برای نمونه:
//         password = "1234";

//         bool ok = dbManager.insertRestaurant(restName, ownerFirst, ownerLast, phone, province, city, password);
//         if (ok) {
//             sender->write("SIGNUP_OK\n");
//             emit logMessage("✅ ثبت رستوران جدید: " + restName);
//         } else {
//             sender->write("SIGNUP_FAIL:خطا در ثبت رستوران\n");
//             emit logMessage("❌ ثبت رستوران ناموفق");
//         }
//     }
//     else if (msg.startsWith("SIGNUP_CUSTOMER:")) {
//         QStringList parts = msg.split(":");
//         if(parts.size() != 5) {
//             sender->write("SIGNUP_FAIL:فرمت اشتباه\n");
//             return;
//         }
//         QString firstName = parts[1].trimmed();
//         QString lastName = parts[2].trimmed();
//         QString phone = parts[3].trimmed();
//         QString password = parts[4].trimmed();

//         bool ok = dbManager.insertCustomer(firstName, lastName, phone, password);
//         if (ok) {
//             sender->write("SIGNUP_OK\n");
//             emit logMessage("✅ ثبت مشتری جدید: " + firstName + " " + lastName);
//         } else {
//             sender->write("SIGNUP_FAIL:خطا در ثبت مشتری\n");
//             emit logMessage("❌ ثبت مشتری ناموفق");
//         }
//     }
//     else {
//         sender->write("ERROR:فرمان ناشناخته\n");
//     }
// }
