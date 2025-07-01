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
        if(parts.size() != 5) {
            sender->write("LOGIN_FAIL:فرمت اشتباه\n");
            return;
        }

        QString role       = parts[1].trimmed().toLower();  // نقش انتخاب‌شده توسط کاربر
        QString firstName  = parts[2].trimmed();
        QString lastName   = parts[3].trimmed();
        QString password   = parts[4].trimmed();

        auto dbRole = dbManager.checkUserLogin(firstName, lastName, password);  // نقش واقعی در دیتابیس

        bool matched = false;
        QString roleStr;

        if (dbRole == DatabaseManager::UserRole::None) {
            sender->write("LOGIN_FAIL:کاربر بلاک شده یا اطلاعات نادرست است\n");
            emit logMessage("❌ ورود ناموفق یا بلاک برای: " + firstName + " " + lastName);
            return;
        }

        switch (dbRole) {
        case DatabaseManager::UserRole::Customer:
            matched = (role == "customer");
            roleStr = "Customer";
            break;
        case DatabaseManager::UserRole::Restaurant:
            matched = (role == "restaurant");
            roleStr = "Restaurant";
            break;
        case DatabaseManager::UserRole::Admin:
            matched = (role == "admin");
            roleStr = "Admin";
            break;

        default:
            matched = false;
        }

        if (matched) {
            sender->write(("LOGIN_OK:" + roleStr + "\n").toUtf8());
            emit logMessage("✅ ورود موفق: " + firstName + " " + lastName + " (" + roleStr + ")");
        } else {
            sender->write("LOGIN_FAIL:نقش اشتباه یا اطلاعات نادرست\n");
            emit logMessage("❌ ورود ناموفق برای: " + firstName + " " + lastName + " با نقش " + role);
        }
    }


    else if (msg.startsWith("SIGNUP_CUSTOMER:")) {
        QStringList parts = msg.split(":");
        if (parts.size() != 5) {
            sender->write("SIGNUP_FAIL:فرمت اشتباه\n");
            return;
        }

        QString firstName = parts[1].trimmed();
        QString lastName = parts[2].trimmed();
        QString phone = parts[3].trimmed();
        QString password = parts[4].trimmed();

        bool ok = dbManager.insertCustomer(firstName, lastName, phone, password);
        if (ok) {
            sender->write("SIGNUP_OK:Customer\n");
            emit logMessage("✅ ثبت مشتری جدید: " + firstName + " " + lastName);
        } else {
            sender->write("SIGNUP_FAIL:Customer\n");
            emit logMessage("❌ ثبت مشتری ناموفق");
        }
    }

    else if (msg.startsWith("SIGNUP_RESTAURANT:")) {
        QStringList parts = msg.split(":");
        if (parts.size() != 9) {
            sender->write("SIGNUP_FAIL:فرمت اشتباه\n");
            return;
        }

        QString restName   = parts[1].trimmed();
        QString ownerFirst = parts[2].trimmed();
        QString ownerLast  = parts[3].trimmed();
        QString phone      = parts[4].trimmed();
        QString province   = parts[5].trimmed();
        QString city       = parts[6].trimmed();
        QString password   = parts[7].trimmed();
        QString type       = parts[8].trimmed();

        bool ok = dbManager.insertRestaurant(restName, ownerFirst, ownerLast, phone, province, city, password, type);

        if (ok) {
            sender->write("SIGNUP_OK:Restaurant\n");
            emit logMessage("✅ ثبت رستوران جدید: " + restName);
        } else {
            sender->write("SIGNUP_FAIL:Restaurant\n");
            emit logMessage("❌ ثبت رستوران ناموفق");
        }
    }

    else if (msg == "GET_RESTAURANTS") {
        QSqlQuery query;
        query.prepare("SELECT restaurant_name, restaurant_type, province, city FROM restaurants");

        if (query.exec()) {
            QStringList restaurantLines;

            while (query.next()) {
                QString name = query.value(0).toString();
                QString type = query.value(1).toString();
                QString province = query.value(2).toString();
                QString city = query.value(3).toString();
                QString fullAddress = province + " - " + city;

                qDebug() << "📦 Restaurant found:" << name << "," << type << "," << fullAddress;

                // ساخت خط رشته‌ای با فرمت name|type|address
                restaurantLines << name + "|" + type + "|" + fullAddress;
            }

            if (!restaurantLines.isEmpty()) {
                QString response = "RESTAURANT_LIST:" + restaurantLines.join(";") + "\n";
                qDebug() << "📤 Sending response:" << response;

                sender->write(response.toUtf8());
                sender->flush(); // ✨ خیلی مهم برای اطمینان از ارسال سریع پیام
            } else {
                qDebug() << "❌ هیچ رستورانی یافت نشد";
                sender->write("RESTAURANT_LIST:EMPTY\n");
                sender->flush();
            }
        } else {
            qDebug() << "❌ خطا در اجرای کوئری: " << query.lastError().text();
            sender->write("RESTAURANT_LIST_FAIL\n");
            sender->flush();
        }
    }


    else {
        sender->write("ERROR:فرمان ناشناخته\n");
    }


}



