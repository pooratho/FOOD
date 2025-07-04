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

    static QMap<QTcpSocket*, QByteArray> bufferMap;  // بافر جدا برای هر کلاینت

    bufferMap[client] += client->readAll();  // داده جدید رو به بافر اضافه کن

    while (bufferMap[client].contains('\n')) {
        int newlineIndex = bufferMap[client].indexOf('\n');
        QByteArray rawMsg = bufferMap[client].left(newlineIndex);
        bufferMap[client] = bufferMap[client].mid(newlineIndex + 1);  // حذف پیام پردازش‌شده

        QString msg = QString::fromUtf8(rawMsg).trimmed();
        if (!msg.isEmpty()) {
            emit logMessage("📨 پیام دریافتی: " + msg);
            processMessage(client, msg);  // پردازش پیام تکی
        }
    }
}

void ServerManager::onDisconnected()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (!client)
        return;

    // پاک کردن سوکت مشتری اگر وجود داره
    int customerIdToRemove = -1;
    for (auto it = customerSockets.begin(); it != customerSockets.end(); ++it) {
        if (it.value() == client) {
            customerIdToRemove = it.key();
            break;
        }
    }
    if (customerIdToRemove != -1) {
        customerSockets.remove(customerIdToRemove);
        qDebug() << "🔴 سوکت مشتری با ID" << customerIdToRemove << "حذف شد";
    }

    // پاک کردن سوکت رستوران اگر در restaurantSocketMap بود
    if (restaurantSocketMap.contains(client)) {
        restaurantSocketMap.remove(client);
        qDebug() << "🔴 سوکت رستوران حذف شد";
    }

    clients.removeAll(client);
    emit logMessage("❌ کلاینت قطع شد: " + client->peerAddress().toString());
    client->deleteLater();
}


QString normalizePhoneNumber(const QString& phone) {
    QString p = phone.trimmed();
    if (!p.startsWith("+98")) {
        if (p.startsWith("0")) {
            p = "+98" + p.mid(1);
        }
    }
    return p;
}
QTcpSocket* ServerManager::findRestaurantSocketById(int restaurantId) {
    for (auto it = restaurantSocketMap.begin(); it != restaurantSocketMap.end(); ++it) {
        if (it.value() == restaurantId) {
            return it.key();  // سوکت کلاینت
        }
    }
    return nullptr;
}

void ServerManager::registerRestaurantSocket(int restaurantId, QTcpSocket* socket) {
    restaurantSocketMap[socket] = restaurantId;
    connectedRestaurantSockets[QString::number(restaurantId)] = socket;
}
void ServerManager::notifyRestaurantNewOrderNotification(int restaurantId)
{
    QTcpSocket* restaurantSocket = findRestaurantSocketById(restaurantId);
    if (!restaurantSocket) {
        qDebug() << "❌ رستوران متصل نیست (نوتیف ساده)";
        return;
    }

    QString message = "NEW_ORDER_ALERT:\n";  // فقط پیام هشدار ساده
    restaurantSocket->write(message.toUtf8());
    restaurantSocket->flush();
    qDebug() << "📨 نوتیف ساده ارسال شد به رستوران ID:" << restaurantId;
}


void ServerManager::notifyCustomerOrderStatusChanged(int orderId, const QString& newStatus)
{

    int customerId = dbManager.getCustomerIdByOrderId(orderId);
    qDebug() << "🎯 در حال بررسی customerId:" << customerId;
    qDebug() << "🎯 کلیدهای customerSockets: " << customerSockets.keys();

    if (!customerSockets.contains(customerId)) {
        qDebug() << "❌ مشتری متصل نیست، نمی‌توان اعلان وضعیت ارسال کرد:" << customerId;
        return;
    }

    QTcpSocket* custSocket = customerSockets[customerId];

    QString message = "ORDER_STATUS_CHANGED:" + QString::number(orderId) + "#" + newStatus;
    qDebug() << "✅ پیام نوتیف برای مشتری:" << message;

    custSocket->write(message.toUtf8() + "\n");
    custSocket->flush();
}


void ServerManager::registerCustomerSocket(int customerId, QTcpSocket* socket) {
    customerSockets[customerId] = socket;
}

//
QTcpSocket* ServerManager::findCustomerSocketById(int customerId) {
    qDebug() << "🔎 در حال جست‌وجوی customerId: " << customerId;
    if (customerSockets.contains(customerId)) {
        qDebug() << "✅ سوکت مشتری یافت شد!";
        return customerSockets[customerId];
    }
    qDebug() << "❌ سوکت مشتری یافت نشد!";
    return nullptr;
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

            if (dbRole == DatabaseManager::UserRole::Restaurant) {
                int restaurantId = dbManager.getRestaurantId(firstName, lastName, password);

                if (restaurantId == -1) {
                    qDebug() << "❌ رستوران یافت نشد برای نام و پسورد داده شده.";
                    sender->write("LOGIN_FAIL:شناسه رستوران یافت نشد\n");
                    return;
                }

                // ثبت سوکت
                registerRestaurantSocket(restaurantId, sender);
                qDebug() << "✅ رستوران لاگین کرد، ID:" << restaurantId;

                QString restaurantName = dbManager.getRestaurantNameById(restaurantId);
                QString response = QString("LOGIN_OK:%1:%2\n").arg(roleStr).arg(restaurantName);
                sender->write(response.toUtf8());

                emit logMessage("🍽️ سوکت رستوران ثبت شد: " + QString::number(restaurantId) + " / " + restaurantName);
            }

            else if (dbRole == DatabaseManager::UserRole::Customer) {
                QString phone = dbManager.getPhoneByName(firstName, lastName);
          qDebug ()<<"رر"<<phone;
                QString response = QString("LOGIN_OK:%1:%2\n").arg(roleStr).arg(phone);
                sender->write(response.toUtf8());
                emit logMessage("✅ ورود موفق: " + firstName + " " + lastName + " (" + roleStr + ")");
                int customerId = dbManager.getCustomerIdByPhone(phone);
                if (customerId != -1) {
                    // سوکت مشتری رو ثبت کن
                    registerCustomerSocket(customerId, sender);
                }
            }
            else {
                // برای سایر نقش‌ها مثل Customer یا Admin فقط نقش بفرست
                sender->write(("LOGIN_OK:" + roleStr + "\n").toUtf8());
                emit logMessage("✅ ورود موفق: " + firstName + " " + lastName + " (" + roleStr + ")");
            }
        }


        else {
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
            int customerId = dbManager.getCustomerIdByPhone(phone);
            if (customerId != -1) {
                // سوکت مشتری رو ثبت کن
                registerCustomerSocket(customerId, sender);
            }
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
        query.prepare("SELECT restaurant_name, restaurant_type, province, city FROM restaurants WHERE is_blocked = 0");

        if (query.exec()) {
            QStringList restaurantLines;
            while (query.next()) {
                QString name = query.value(0).toString();
                QString type = query.value(1).toString();
                QString province = query.value(2).toString();
                QString city = query.value(3).toString();
                QString fullAddress = province + " - " + city;

                restaurantLines << name + "|" + type + "|" + fullAddress;
            }

            if (!restaurantLines.isEmpty()) {
                QString response = "RESTAURANT_LIST:" + restaurantLines.join(";") + "\n";
                sender->write(response.toUtf8());
                sender->flush();
            } else {
                sender->write("RESTAURANT_LIST:EMPTY\n");
                sender->flush();
            }
        } else {
            sender->write("RESTAURANT_LIST_FAIL\n");
            sender->flush();
        }
    }

    else if (msg.startsWith("GET_RESTAURANTS_FILTERED:")) {

        QString filterPart = msg.mid(QString("GET_RESTAURANTS_FILTERED:").length()).trimmed();

        // پارس کردن فیلترها
        QMap<QString, QString> filters;
        QStringList pairs = filterPart.split(";", Qt::SkipEmptyParts);
        for (const QString &pair : pairs) {
            QStringList keyVal = pair.split("=", Qt::SkipEmptyParts);
            if (keyVal.size() == 2) {
                filters[keyVal[0].trimmed().toLower()] = keyVal[1].trimmed();
            }
        }

        // ساخت کوئری داینامیک با توجه به فیلترها
        QString baseQuery = "SELECT restaurant_name, restaurant_type, province, city FROM restaurants WHERE is_blocked = 0";
        QStringList conditions;

        if (filters.contains("province") && !filters["province"].isEmpty())
            conditions << "province = :province";
        if (filters.contains("city") && !filters["city"].isEmpty())
            conditions << "city = :city";
        if (filters.contains("type") && !filters["type"].isEmpty())
            conditions << "restaurant_type = :type";

        if (!conditions.isEmpty()) {
            baseQuery += " AND " + conditions.join(" AND ");
        }

        QSqlQuery query;
        query.prepare(baseQuery);

        // مقداردهی پارامترها
        if (filters.contains("province") && !filters["province"].isEmpty())
            query.bindValue(":province", filters["province"]);
        if (filters.contains("city") && !filters["city"].isEmpty())
            query.bindValue(":city", filters["city"]);
        if (filters.contains("type") && !filters["type"].isEmpty())
            query.bindValue(":type", filters["type"]);

        if (query.exec()) {
            QStringList restaurantLines;
            while (query.next()) {
                QString name = query.value(0).toString();
                QString type = query.value(1).toString();
                QString province = query.value(2).toString();
                QString city = query.value(3).toString();
                QString fullAddress = province + " - " + city;

                qDebug() << "📦 Restaurant found:" << name << "," << type << "," << fullAddress;

                restaurantLines << name + "|" + type + "|" + fullAddress;
            }

            if (!restaurantLines.isEmpty()) {
                QString response = "RESTAURANT_LIST:" + restaurantLines.join(";") + "\n";
                qDebug() << "📤 Sending filtered response:" << response;

                sender->write(response.toUtf8());
                sender->flush();
            } else {
                qDebug() << "❌ هیچ رستورانی با فیلترهای داده شده یافت نشد";
                sender->write("RESTAURANT_LIST:EMPTY\n");
                sender->flush();
            }
        } else {
            qDebug() << "❌ خطا در اجرای کوئری فیلتر شده: " << query.lastError().text();
            sender->write("RESTAURANT_LIST_FAIL\n");
            sender->flush();
        }
    }

    else if (msg.startsWith("ADD_FOOD:")) {
        QString content = msg.mid(QString("ADD_FOOD:").length());
        QStringList parts = content.split(":", Qt::KeepEmptyParts);

        if (parts.size() < 4) {
            sender->write("ADD_FOOD_FAIL:فرمت اشتباه\n");
            return;
        }

        QString category = parts[0].trimmed();
        QString name = parts[1].trimmed();
        QString priceStr = parts[2].trimmed();
        // توضیحات ممکنه چند بخش داشته باشه، همه رو اینجا ترکیب کن
        QString desc = parts.mid(3).join(":").trimmed();

        bool ok;
        double price = priceStr.toDouble(&ok);
        if (!ok) {
            sender->write("ADD_FOOD_FAIL:قیمت نامعتبر\n");
            return;
        }


        // مثال: درج در جدول foods
        QSqlQuery query;
        query.prepare("INSERT INTO foods (name, description, price, category, restaurant_id) "
                      "VALUES (:name, :desc, :price, :cat, :rest_id)");
        query.bindValue(":name", name);
        query.bindValue(":desc", desc);
        query.bindValue(":price", price);
        query.bindValue(":cat", category);

        // فعلاً ساده فرض کنیم id رستوران ثابت باشه یا از socket map بگیریم:
        qDebug() << "Current restaurantSocketMap keys:" << restaurantSocketMap.keys();
        qDebug() << "Checking sender in map:" << restaurantSocketMap.contains(sender);
        qDebug() << "restaurantId for sender:" << restaurantSocketMap.value(sender, -1);
        int restaurantId = restaurantSocketMap.value(sender, -1);
        if (restaurantId == -1) {
            sender->write("ADD_FOOD_FAIL:شناسه رستوران یافت نشد\n");
            return;
        }
        query.bindValue(":rest_id", restaurantId);

        if (query.exec()) {
            sender->write("ADD_FOOD_OK\n");
        } else {
            sender->write("ADD_FOOD_FAIL:" + query.lastError().text().toUtf8() + "\n");
        }
    }

    else if (msg == "GET_MENU") {
        int restaurantId = restaurantSocketMap.value(sender, -1);
        if (restaurantId == -1) {
            sender->write("MENU_LIST_FAIL:شناسه رستوران یافت نشد\n");
            return;
        }

        QSqlQuery query;
        query.prepare("SELECT name, description, price, category FROM foods WHERE restaurant_id = :id");
        query.bindValue(":id", restaurantId);

        if (query.exec()) {
            QStringList items;
            while (query.next()) {
                QString name = query.value(0).toString();
                QString desc = query.value(1).toString();
                double price = query.value(2).toDouble();
                QString cat = query.value(3).toString();

                items << name + "|" + desc + "|" + QString::number(price) + "|" + cat;
            }

            QString response = "MENU_LIST:" + items.join(";") + "\n";
            sender->write(response.toUtf8());
            sender->flush();
        } else {
            sender->write("MENU_LIST_FAIL\n");
        }
    }
    else if (msg.startsWith("REGISTER_RESTAURANT_SOCKET:")) {
        QString restaurantName = msg.mid(QString("REGISTER_RESTAURANT_SOCKET:").length()).trimmed();
        qDebug() << "Received REGISTER_RESTAURANT_SOCKET with name:" << restaurantName;

        int restId = dbManager.getRestaurantIdByRestaurantName(restaurantName);
        qDebug() << "Got restaurant ID from DB:" << restId;

        if (restId != -1) {
            restaurantSocketMap[sender] = restId;
            sender->write("REGISTER_OK\n");
        } else {
            sender->write("REGISTER_FAIL:شناسه پیدا نشد\n");
        }

    }
    else if (msg.startsWith("DELETE_FOOD:")) {
        QStringList parts = msg.split(":");
        if (parts.size() != 3) {
            sender->write("DELETE_FOOD_FAIL:فرمت اشتباه\n");
            return;
        }

        QString category = parts[1].trimmed();
        QString foodName = parts[2].trimmed();

        bool success = dbManager.deleteFood(category, foodName); // این تابع رو می‌سازیم پایین
        if (success) {
            sender->write("DELETE_FOOD_OK\n");
            emit logMessage("🍽️ غذا حذف شد: " + foodName + " (" + category + ")");
        } else {
            sender->write("DELETE_FOOD_FAIL:حذف از دیتابیس ناموفق بود\n");
        }
    }

    else if (msg.startsWith("ADD_TO_CART:")) {
        QString data = msg.mid(QString("ADD_TO_CART:").length());
        QStringList parts = data.split("#");
        if (parts.size() != 2) {
            sender->write("ADD_TO_CART_FAIL:فرمت نادرست\n");
            return;
        }

       QString phone = normalizePhoneNumber(parts[0].trimmed());
        QStringList itemParts = parts[1].split("|");


        if (itemParts.size() != 4) {
            sender->write("ADD_TO_CART_FAIL:فرمت آیتم نادرست\n");
            return;
        }

        QString restaurantName = itemParts[0].trimmed();
        QString foodName = itemParts[1].trimmed();
        int quantity = itemParts[2].toInt();
        double unitPrice = itemParts[3].toDouble();

        // پیدا کردن id مشتری از روی شماره تلفن
        QSqlQuery query;
           qDebug() << "Phone received for add to cart:" << phone;
        query.prepare("SELECT id FROM customers WHERE phone = ?");
        query.addBindValue(phone);

        if (!query.exec() || !query.next()) {
            sender->write("ADD_TO_CART_FAIL:مشتری یافت نشد\n");
            return;
        }

        int customerId = query.value(0).toInt();

        qDebug() << "Customer ID found:" << customerId;
        // اضافه کردن آیتم به جدول cart_items
        QSqlQuery insertQuery;
        insertQuery.prepare("INSERT INTO cart_items (customer_id, restaurant_name, food_name, quantity, unit_price) "
                            "VALUES (?, ?, ?, ?, ?)");
        insertQuery.addBindValue(customerId);
        insertQuery.addBindValue(restaurantName);
        insertQuery.addBindValue(foodName);
        insertQuery.addBindValue(quantity);
        insertQuery.addBindValue(unitPrice);

        if (!insertQuery.exec()) {
            sender->write("ADD_TO_CART_FAIL:خطا در درج\n");
            return;
        }

        sender->write("ADD_TO_CART_OK\n");
    }

    else if (msg.startsWith("REMOVE_FROM_CART:")) {
        QString data = msg.mid(QString("REMOVE_FROM_CART:").length());
        QStringList parts = data.split("#");
        if (parts.size() != 2) {
            sender->write("REMOVE_CART_FAIL:فرمت نادرست\n");
            return;
        }

        QString phone = normalizePhoneNumber(parts[0].trimmed());
        QStringList itemParts = parts[1].split("|");
        if (itemParts.size() != 2) {
            sender->write("REMOVE_CART_FAIL:آیتم نادرست\n");
            return;
        }

        QString restaurantName = itemParts[0].trimmed();
        QString foodName = itemParts[1].trimmed();

        QSqlQuery query;
        query.prepare("SELECT id FROM customers WHERE phone = ?");
        query.addBindValue(phone);

        if (!query.exec() || !query.next()) {
            sender->write("REMOVE_CART_FAIL:مشتری یافت نشد\n");
            return;
        }

        int customerId = query.value(0).toInt();

        QSqlQuery deleteQuery;
        deleteQuery.prepare("DELETE FROM cart_items WHERE customer_id = ? AND restaurant_name = ? AND food_name = ?");
        deleteQuery.addBindValue(customerId);
        deleteQuery.addBindValue(restaurantName);
        deleteQuery.addBindValue(foodName);

        if (!deleteQuery.exec()) {
            sender->write("REMOVE_CART_FAIL:حذف ناموفق\n");
            return;
        }

        sender->write("REMOVE_CART_OK\n");
    }


    else if (msg.startsWith("GET_CART:")) {
        QString phone = msg.mid(QString("GET_CART:").length()).trimmed();

        QList<DatabaseManager::CartItem> items = dbManager.getCartItemsByPhone(phone);
        if (items.isEmpty()) {
            sender->write("GET_CART_FAIL:هیچ آیتمی یافت نشد\n");
            return;
        }

        QStringList serializedItems;
        for (const auto& item : items) {
            // فرمت: restaurantName|foodName|quantity|unitPrice
            serializedItems.append(item.restaurantName + "|" +
                                   item.foodName + "|" +
                                   QString::number(item.quantity) + "|" +
                                   QString::number(item.unitPrice));
        }

        QString response = "GET_CART_OK:" + serializedItems.join("#") + "\n";
        sender->write(response.toUtf8());
    }
    else if (msg.startsWith("UPDATE_CART:")) {
        QString data = msg.mid(QString("UPDATE_CART:").length()).trimmed();
        qDebug() << "📥 UPDATE_CART raw data:" << data;

        QStringList parts = data.split("#");
        if (parts.size() != 2) {
            sender->write("UPDATE_CART_FAIL:فرمت نادرست\n");
            return;
        }

        QString phone = normalizePhoneNumber(parts[0].trimmed());
        QStringList itemParts = parts[1].split("|");

        if (itemParts.size() != 3) {
            sender->write("UPDATE_CART_FAIL:فرمت نادرست\n");
            return;
        }

        QString restaurant = itemParts[0].trimmed();
        QString food = itemParts[1].trimmed();

        bool ok = false;
        int quantity = itemParts[2].toInt(&ok);

        if (!ok || quantity < 0 || restaurant.isEmpty() || food.isEmpty()) {
            sender->write("UPDATE_CART_FAIL:مقادیر نامعتبر\n");
            return;
        }

        // گرفتن customer_id از روی شماره تلفن
        QSqlQuery query;
        query.prepare("SELECT id FROM customers WHERE phone = ?");
        query.addBindValue(phone);

        if (!query.exec() || !query.next()) {
            sender->write("UPDATE_CART_FAIL:مشتری یافت نشد\n");
            return;
        }

        int customerId = query.value(0).toInt();

        // به‌روزرسانی مقدار quantity (قیمت رو تغییر نمی‌دیم)
        QSqlQuery updateQuery;
        updateQuery.prepare(R"(
        INSERT INTO cart_items (customer_id, restaurant_name, food_name, quantity, unit_price)
        VALUES (?, ?, ?, ?, 0)
        ON CONFLICT(customer_id, restaurant_name, food_name)
        DO UPDATE SET quantity = excluded.quantity
    )");

        updateQuery.addBindValue(customerId);
        updateQuery.addBindValue(restaurant);
        updateQuery.addBindValue(food);
        updateQuery.addBindValue(quantity);

        if (!updateQuery.exec()) {
            qDebug() << "❌ UPDATE_CART خطا در اجرا:" << updateQuery.lastError().text();
            sender->write("UPDATE_CART_FAIL:خطا در به‌روزرسانی\n");
            return;
        }

        sender->write("UPDATE_CART_OK:به‌روزرسانی موفق\n");
    }

    else if (msg.startsWith("SUBMIT_ORDER:")) {
        QString data = msg.mid(QString("SUBMIT_ORDER:").length()).trimmed();
        QStringList parts = data.split("#");
        if (parts.size() != 2) {
            sender->write("SUBMIT_ORDER_FAIL:فرمت نادرست\n");
            return;
        }

        QString phone = normalizePhoneNumber(parts[0].trimmed());
        QStringList orderParts = parts[1].split("|");

        if (orderParts.isEmpty()) {
            sender->write("SUBMIT_ORDER_FAIL:اطلاعات سفارش خالی است\n");
            return;
        }

        QString restaurantName = orderParts[0].trimmed();
        int customerId = dbManager.getCustomerIdByPhone(phone);
        if (customerId == -1) {
            sender->write("SUBMIT_ORDER_FAIL:مشتری یافت نشد\n");
            return;
        }

        int restaurantId = dbManager.getRestaurantIdByRestaurantName(restaurantName);
        if (restaurantId == -1) {
            sender->write("SUBMIT_ORDER_FAIL:رستوران یافت نشد\n");
            return;
        }

        double totalPrice = 0;
        QList<DatabaseManager::TempCartItem> items;

        for (int i = 1; i < orderParts.size(); ++i) {
            QStringList foodData = orderParts[i].split(",");
            if (foodData.size() != 3) continue;

            QString foodName = foodData[0].trimmed();
            int quantity = foodData[1].toInt();
            double price = foodData[2].toDouble();
            totalPrice += quantity * price;

            items.append({restaurantId, foodName, quantity, price});
        }

        if (!dbManager.submitSplitOrder(customerId, totalPrice, items)) {
            sender->write("SUBMIT_ORDER_FAIL:ثبت سفارش ناموفق\n");
            return;
        }

        DatabaseManager::OrderData newOrder = dbManager.getLastOrderForCustomer(customerId);

        // فقط یک نوتیف ساده ارسال کن
        notifyRestaurantNewOrderNotification(restaurantId);

        dbManager.clearCartByCustomerId(customerId);

        sender->write("SUBMIT_ORDER_OK\n");
        sender->flush();

    }
    else if (msg.startsWith("GET_CUSTOMER_ORDERS:")) {
        QString phone = normalizePhoneNumber(msg.mid(QString("GET_CUSTOMER_ORDERS:").length()).trimmed());
        int customerId = dbManager.getCustomerIdByPhone(phone);

        if (customerId == -1) {
            sender->write("ORDERS_FAIL:مشتری یافت نشد\n");
            return;
        }

        QList<DatabaseManager::OrderData> orders = dbManager.getOrdersByCustomerId(customerId);

        if (orders.isEmpty()) {
            sender->write("ORDERS_EMPTY:سفارشی یافت نشد\n");
            return;
        }

        for (const DatabaseManager::OrderData& order : orders) {
            QString message = "ORDER_ITEM:" +
                              dbManager.getRestaurantNameById(order.restaurantId) + "#" +
                              QString::number(order.orderId) + "|" +
                              QString::number(order.totalPrice) + "|" +
                              order.status + "|" +
                              order.createdAt;
            for (const auto& item : order.items) {
                message += "|" + item.foodName + "," +
                           QString::number(item.quantity) + "," +
                           QString::number(item.unitPrice);
            }

            sender->write(message.toUtf8() + "\n");
        }

        sender->write("ORDERS_DONE\n");
    }
    else if (msg.startsWith("GET_RESTAURANT_ORDERS:")) {
        QString restName = msg.mid(QString("GET_RESTAURANT_ORDERS:").length()).trimmed();
        int restId = dbManager.getRestaurantIdByRestaurantName(restName);

        if (restId == -1) {
            sender->write("REST_ORDERS_FAIL:رستوران یافت نشد\n");
            return;
        }

        QList<DatabaseManager::OrderData> orders = dbManager.getOrdersByRestaurantId(restId);

        if (orders.isEmpty()) {
            sender->write("REST_ORDERS_EMPTY:سفارشی برای رستوران یافت نشد\n");
            return;
        }

        for (const DatabaseManager::OrderData& order : orders) {
            QString customerPhone = dbManager.getCustomerPhoneById(order.customerId);

            QString message = "RESTAURANT_ORDER:" +
                              customerPhone + "#" +                    // شماره مشتری
                              QString::number(order.orderId) + "|" +  // اینجا orderId اضافه شد
                              QString::number(order.totalPrice) + "|" +
                              order.status + "|" +
                              order.createdAt;

            for (const auto& item : order.items) {
                message += "|" + item.foodName + "," +
                           QString::number(item.quantity) + "," +
                           QString::number(item.unitPrice);
            }

            sender->write(message.toUtf8() + "\n");
        }

        sender->write("REST_ORDERS_DONE\n");
    }
    else if (msg.startsWith("UPDATE_ORDER_STATUS:")) {
        QString data = msg.mid(QString("UPDATE_ORDER_STATUS:").length()).trimmed();
        QStringList parts = data.split("#");
        if (parts.size() != 2) {
            sender->write("UPDATE_STATUS_FAIL:فرمت پیام نادرست است\n");
            return;
        }

        int orderId = parts[0].toInt();
        QString newStatus = parts[1];

        qDebug() << "UpdateOrderStatus request received. orderId:" << orderId << "newStatus:" << newStatus;

        bool success = dbManager.updateOrderStatus(orderId, newStatus);
        if (success) {
            sender->write("UPDATE_STATUS_OK:\n");
            notifyCustomerOrderStatusChanged(orderId, newStatus);
            qDebug() << "Order status updated successfully.";

            // ارسال پیام به مشتری
            int customerId = dbManager.getCustomerIdByOrderId(orderId);
            qDebug() << "✅ customerId: " << customerId;
            //
            qDebug() << "🧪 customerId to find: " << customerId;
            qDebug() << "🧪 current customerSockets keys: " << customerSockets.keys();

            QTcpSocket* customerSocket = findCustomerSocketById(customerId);
            if (customerSocket) {
                QString notifyMsg = QString("ORDER_STATUS_UPDATED:%1|%2\n").arg(orderId).arg(newStatus);
                qDebug() << "✅ ارسال به مشتری: " << notifyMsg;
                customerSocket->write(notifyMsg.toUtf8());
            }
            else {
                qDebug() << "❌ مشتری آنلاین نیست یا سوکت یافت نشد";
            }


        }
      else {
            sender->write("UPDATE_STATUS_FAIL:به‌روزرسانی وضعیت ناموفق بود\n");
            qDebug() << "Failed to update order status!";
        }
    }


    else {
        sender->write("ERROR:فرمان ناشناخته\n");
    }


}



