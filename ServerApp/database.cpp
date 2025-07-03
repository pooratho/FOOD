#include "database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QDebug>

DatabaseManager::DatabaseManager() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("mydatabase.db");
    if (!db.open()) {
        qDebug() << "Failed to open DB in constructor:" << db.lastError().text();
    } else {
        createTables();  // جدول‌ها رو اینجا بساز
        qDebug() << "Database opened and tables created successfully.";
    }
}


DatabaseManager::~DatabaseManager() {
    if (db.isOpen())
        db.close();
}

bool DatabaseManager::connectToDatabase() {
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("mydatabase.db");
    }

    if (!db.open()) {
        qDebug() << "Failed to open DB:" << db.lastError().text();
        return false;
    }
    return true;
}

void DatabaseManager::createTables() {
    QSqlQuery query(db);

    // جدول مشتری
    bool successCustomer = query.exec(
        "CREATE TABLE IF NOT EXISTS customers ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "firstname TEXT NOT NULL, "
        "lastname TEXT NOT NULL, "
        "phone TEXT NOT NULL, "
        "password TEXT NOT NULL, "
        "is_blocked INTEGER DEFAULT 0)" // ✅ اضافه شد
        );
    if (!successCustomer)
        qDebug() << "Create customers table error:" << query.lastError().text();

    // جدول رستوران
    bool successRestaurant = query.exec(
        "CREATE TABLE IF NOT EXISTS restaurants ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "restaurant_name TEXT NOT NULL, "
        "owner_firstname TEXT NOT NULL, "
        "owner_lastname TEXT NOT NULL, "
        "phone TEXT NOT NULL, "
        "province TEXT NOT NULL, "
        "city TEXT NOT NULL, "
        "password TEXT NOT NULL, "
        "restaurant_type TEXT NOT NULL, "
        "is_blocked INTEGER DEFAULT 0)"
        );

    if (!successRestaurant)
        qDebug() << "Create restaurants table error:" << query.lastError().text();

    // جدول مدیر
    bool successAdmin = query.exec(
        "CREATE TABLE IF NOT EXISTS admins ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "firstname TEXT NOT NULL, "
        "lastname TEXT NOT NULL, "
        "password TEXT NOT NULL)"
        );
    if (!successAdmin)
        qDebug() << "Create admins table error:" << query.lastError().text();

    // افزودن ادمین پیش‌فرض
    query.prepare("SELECT COUNT(*) FROM admins WHERE firstname = 'ADMIN' AND lastname = 'ADMIN'");
    if (query.exec() && query.next() && query.value(0).toInt() == 0) {
        query.prepare("INSERT INTO admins (firstname, lastname, password) "
                      "VALUES (:firstname, :lastname, :password)");
        query.bindValue(":firstname", "ADMIN");
        query.bindValue(":lastname", "ADMIN");
        query.bindValue(":password", hashPassword("ADMIN"));
        if (!query.exec()) {
            qDebug() << "Insert default admin failed:" << query.lastError().text();
        }
    }

    // جدول غذاها (منو)
    bool successFoods = query.exec(
        "CREATE TABLE IF NOT EXISTS foods ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT NOT NULL, "
        "description TEXT, "
        "price REAL NOT NULL, "
        "category TEXT NOT NULL, "
        "restaurant_id INTEGER NOT NULL, "
        "FOREIGN KEY (restaurant_id) REFERENCES restaurants(id))"
        );

    if (!successFoods)
        qDebug() << "Create foods table error:" << query.lastError().text();
    // جدول سفارش‌ها
    bool successOrder = query.exec(
        "CREATE TABLE IF NOT EXISTS orders ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "customer_id INTEGER NOT NULL, "
        "total_price REAL NOT NULL, "
        "status TEXT DEFAULT 'ثبت شده', "
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP, "
        "FOREIGN KEY (customer_id) REFERENCES customers(id))"
        );
    if (!successOrder)
        qDebug() << "Create orders table error:" << query.lastError().text();

    // جدول آیتم‌های سفارش
    bool successOrderItems = query.exec(
        "CREATE TABLE IF NOT EXISTS order_items ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "order_id INTEGER NOT NULL, "
        "restaurant_id INTEGER NOT NULL, "
        "food_name TEXT NOT NULL, "
        "quantity INTEGER NOT NULL, "
        "unit_price REAL NOT NULL, "
        "FOREIGN KEY (order_id) REFERENCES orders(id))"
        );
    if (!successOrderItems)
        qDebug() << "Create order_items table error:" << query.lastError().text();
    bool successCart = query.exec(
        "CREATE TABLE IF NOT EXISTS cart_items ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "customer_id INTEGER NOT NULL, "
        "restaurant_name TEXT NOT NULL, "
        "food_name TEXT NOT NULL, "
        "quantity INTEGER NOT NULL, "
        "unit_price REAL NOT NULL, "
        "FOREIGN KEY (customer_id) REFERENCES customers(id), "
        "UNIQUE (customer_id, restaurant_name, food_name))"
        );
    if (!successCart)
        qDebug() << "Create cart_items table error:" << query.lastError().text();


}


// void DatabaseManager::creatRestaurantTable() {
//     QSqlQuery query(db);
//     bool success = query.exec(
//         "CREATE TABLE IF NOT EXISTS customers ("
//         "id INTEGER PRIMARY KEY AUTOINCREMENT, "
//         "firstname TEXT NOT NULL, "
//         "lastname TEXT NOT NULL, "
//         "phone TEXT NOT NULL, "
//         "password TEXT NOT NULL)"
//         );

//     if (!success) {
//         qDebug() << "Create table error:" << query.lastError().text();
//     }
// }


bool DatabaseManager::insertCustomer(const QString& firstName, const QString& lastName, const QString& phone, const QString& password) {
    QSqlQuery query(db);


    query.prepare("INSERT INTO customers (firstname, lastname, phone, password, is_blocked) "
                  "VALUES (:firstname, :lastname, :phone, :password, 0)");

    query.bindValue(":firstname", firstName);
    query.bindValue(":lastname", lastName);
    query.bindValue(":phone", phone);
    query.bindValue(":password", hashPassword(password));

    if (!query.exec()) {
        QMessageBox::warning(nullptr, "Insert Customer Error", query.lastError().text());
        return false;
    }
    return true;
}

bool DatabaseManager::insertRestaurant(const QString& restaurantName,
                                       const QString& ownerFirstName,
                                       const QString& ownerLastName,
                                       const QString& phone,
                                       const QString& province,
                                       const QString& city,
                                       const QString& password,
                                       const QString& restaurantType)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO restaurants "
                  "(restaurant_name, owner_firstname, owner_lastname, phone, province, city, password, restaurant_type, is_blocked) "
                  "VALUES (:restaurant_name, :owner_firstname, :owner_lastname, :phone, :province, :city, :password, :restaurant_type, 0)");

    qDebug() << query.lastQuery();

    query.bindValue(":restaurant_name", restaurantName);
    query.bindValue(":owner_firstname", ownerFirstName);
    query.bindValue(":owner_lastname", ownerLastName);
    query.bindValue(":phone", phone);
    query.bindValue(":province", province);
    query.bindValue(":city", city);
    query.bindValue(":password", hashPassword(password));
    query.bindValue(":restaurant_type", restaurantType); // 👈 اینم اضافه بشه

    if (!query.exec()) {
        QMessageBox::warning(nullptr, "Insert Restaurant Error", query.lastError().text());
        return false;
    }
    return true;
}


QString DatabaseManager::hashPassword(const QString& password) {
    QByteArray hashed = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    return hashed.toHex();
}


DatabaseManager::UserRole DatabaseManager::checkUserLogin(const QString& firstName, const QString& lastName, const QString& password) {
    QString hashed = hashPassword(password);
    QSqlQuery query(db);

    // بررسی مشتری (و بلاک بودن)
    query.prepare("SELECT is_blocked FROM customers WHERE firstname = :first AND lastname = :last AND password = :pass");
    query.bindValue(":first", firstName);
    query.bindValue(":last", lastName);
    query.bindValue(":pass", hashed);
    if (query.exec() && query.next()) {
        if (query.value(0).toInt() == 1) {
            return UserRole::None;
        }
        return UserRole::Customer;
    }

    // بررسی رستوران‌دار (و بلاک بودن)
    query.prepare("SELECT is_blocked FROM restaurants WHERE owner_firstname = :first AND owner_lastname = :last AND password = :pass");
    query.bindValue(":first", firstName);
    query.bindValue(":last", lastName);
    query.bindValue(":pass", hashed);
    if (query.exec() && query.next()) {
        if (query.value(0).toInt() == 1) {
            return UserRole::None;
        }
        return UserRole::Restaurant;
    }

    // بررسی مدیر
    query.prepare("SELECT COUNT(*) FROM admins WHERE firstname = :first AND lastname = :last AND password = :pass");
    query.bindValue(":first", firstName);
    query.bindValue(":last", lastName);
    query.bindValue(":pass", hashed);
    if (query.exec() && query.next() && query.value(0).toInt() > 0) {
        return UserRole::Admin;
    }

    return UserRole::None;
}

int DatabaseManager::getRestaurantId(const QString& firstName, const QString& lastName, const QString& password) {
    QString hashedPass = hashPassword(password);

    QSqlQuery query;
    query.prepare("SELECT id FROM restaurants WHERE owner_firstname = :first AND owner_lastname = :last AND password = :pass LIMIT 1");
    query.bindValue(":first", firstName);
    query.bindValue(":last", lastName);
    query.bindValue(":pass", hashedPass);

    if (!query.exec()) {
        qDebug() << "Error in getRestaurantIdByOwner query:" << query.lastError().text();
        return -1;
    }

    if (query.next()) {
        int id = query.value(0).toInt();
        qDebug() << "Found restaurant ID:" << id;
        return id;
    } else {
        qDebug() << "No restaurant found for given owner info";
        return -1;
    }
}

int DatabaseManager::getRestaurantIdByRestaurantName(const QString& restaurantName)
{
    QSqlQuery query;
    query.prepare("SELECT id FROM restaurants WHERE TRIM(restaurant_name) = :name LIMIT 1");
    query.bindValue(":name", restaurantName.trimmed());

    if (!query.exec()) {
        qDebug() << "Error in getRestaurantIdByRestaurantName:" << query.lastError().text();
        return -1;
    }

    if (query.next()) {
        int id = query.value(0).toInt();
        qDebug() << "Found restaurant ID:" << id;
        return id;
    } else {
        qDebug() << "No restaurant found for name:" << restaurantName;
        return -1;
    }
}

QString DatabaseManager::getRestaurantNameById(int restaurantId) {
    QSqlQuery query(db);
    query.prepare("SELECT restaurant_name FROM restaurants WHERE id = :id LIMIT 1");
    query.bindValue(":id", restaurantId);
    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }
    return QString();
}

bool DatabaseManager::deleteFood(const QString& category, const QString& name) {
    QSqlQuery query(db);
    query.prepare("DELETE FROM Foods WHERE category = :category AND name = :name");
    query.bindValue(":category", category);
    query.bindValue(":name", name);

    if (!query.exec()) {
        qDebug() << "خطا در حذف غذا:" << query.lastError().text();
        return false;
    }
    return true;
}


bool DatabaseManager::addOrUpdateCartItem(int customerId,
                                          const QString& restaurant,
                                          const QString& food,
                                          int qty,
                                          double price) {
    QSqlQuery q;
    q.prepare(R"(
        INSERT INTO cart_items
            (customer_id, restaurant_name, food_name, quantity, unit_price)
        VALUES (?,       ?,               ?,         ?,        ?)
        ON CONFLICT(customer_id, restaurant_name, food_name)
        DO UPDATE SET
            quantity   = excluded.quantity,
            unit_price = excluded.unit_price
    )");
    q.addBindValue(customerId);
    q.addBindValue(restaurant);
    q.addBindValue(food);
    q.addBindValue(qty);
    q.addBindValue(price);

    if (!q.exec()) {
        qDebug() << "upsert cart_items error:" << q.lastError().text();
        return false;
    }
    return true;
}


bool DatabaseManager::addOrUpdateCartItemByPhone(const QString& phone,
                                                 const QString& restaurantName,
                                                 const QString& foodName,
                                                 int quantity,
                                                 double unitPrice)
{
    QSqlQuery customerQuery;
    customerQuery.prepare("SELECT id FROM customers WHERE phone = ?");
    customerQuery.addBindValue(phone);

    if (!customerQuery.exec() || !customerQuery.next()) {
        qDebug() << "❌ پیدا نکردن مشتری با شماره تلفن:" << phone << customerQuery.lastError().text();
        return false;
    }

    int customerId = customerQuery.value(0).toInt();

    return addOrUpdateCartItem(customerId, restaurantName, foodName, quantity, unitPrice);
}

bool DatabaseManager::removeCartItem(int customerId,
                                     const QString& restaurantName,
                                     const QString& foodName)
{
    QSqlQuery query;

    if (!query.exec()) {
        query.prepare("DELETE FROM cart_items WHERE customer_id = ? AND restaurant_name = ? AND food_name = ?");
        query.addBindValue(customerId);
        query.addBindValue(restaurantName);
        query.addBindValue(foodName);
        qDebug() << "Remove cart item error:" << query.lastError().text();
        return false;
    }
    return true;
}



QList<DatabaseManager::CartItem> DatabaseManager::getCartItemsByCustomerId(int customerId) {
    QList<CartItem> list;
    QSqlQuery query;
    query.prepare("SELECT restaurant_name, food_name, quantity, unit_price FROM cart_items WHERE customer_id = ?");
    query.addBindValue(customerId);

    if (!query.exec()) {
        qDebug() << "Get cart items error:" << query.lastError().text();
        return list;
    }

    while (query.next()) {
        QString restName = query.value(0).toString();
        QString foodName = query.value(1).toString();
        int quantity = query.value(2).toInt();
        double unitPrice = query.value(3).toDouble();

        list.append(CartItem(foodName, restName, quantity, unitPrice));
    }
    return list;
}

QList<DatabaseManager::CartItem> DatabaseManager::getCartItemsByPhone(const QString& phone)
{
    QSqlQuery customerQuery;
    customerQuery.prepare("SELECT id FROM customers WHERE phone = ?");
    customerQuery.addBindValue(phone);

    if (!customerQuery.exec() || !customerQuery.next()) {
        qDebug() << "❌ پیدا نکردن مشتری برای گرفتن سبد خرید:" << phone << customerQuery.lastError().text();
        return {};
    }

    int customerId = customerQuery.value(0).toInt();
    return  getCartItemsByCustomerId(customerId);
}


QString DatabaseManager::getPhoneByName(const QString& firstName, const QString& lastName) {
    QSqlQuery query;
    query.prepare("SELECT phone FROM customers WHERE firstname = ? AND lastname = ?");
    query.addBindValue(firstName);
    query.addBindValue(lastName);

    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }
    return "";
}

bool DatabaseManager::submitSplitOrder(int customerId, double totalPrice, const QList<TempCartItem>& items)
{
    QSqlQuery insertOrder;
    insertOrder.prepare("INSERT INTO orders (customer_id, total_price) VALUES (?, ?)");
    insertOrder.addBindValue(customerId);
    insertOrder.addBindValue(totalPrice);

    if (!insertOrder.exec()) {
        qDebug() << "❌ خطا در درج سفارش:" << insertOrder.lastError().text();
        return false;
    }

    int orderId = insertOrder.lastInsertId().toInt();

    for (const TempCartItem& item : items) {
        QSqlQuery insertItem;
        insertItem.prepare("INSERT INTO order_items (order_id, restaurant_id, food_name, quantity, unit_price) "
                           "VALUES (?, ?, ?, ?, ?)");
        insertItem.addBindValue(orderId);
        insertItem.addBindValue(item.restaurantId);
        insertItem.addBindValue(item.foodName);
        insertItem.addBindValue(item.quantity);
        insertItem.addBindValue(item.unitPrice);

        if (!insertItem.exec()) {
            qDebug() << "❌ خطا در درج آیتم سفارش:" << insertItem.lastError().text();
            return false;
        }
    }

    return true;
}


int DatabaseManager::getCustomerIdByPhone(const QString& phone)
{
    QSqlQuery query;
    query.prepare("SELECT id FROM customers WHERE phone = ?");
    query.addBindValue(phone);

    if (!query.exec()) {
        qDebug() << "خطا در اجرای کوئری getCustomerIdByPhone:" << query.lastError().text();
        return -1;
    }

    if (query.next()) {
        return query.value(0).toInt();
    } else {
        // مشتری پیدا نشد
        return -1;
    }
}

bool DatabaseManager::clearCartByCustomerId(int customerId)
{
    QSqlQuery query;
    query.prepare("DELETE FROM cart_items WHERE customer_id = ?");
    query.addBindValue(customerId);

    if (!query.exec()) {
        qDebug() << "❌ خطا در پاک کردن سبد خرید:" << query.lastError().text();
        return false;
    }
    return true;
}

QList<DatabaseManager::OrderData> DatabaseManager::getOrdersByCustomerId(int customerId) {
    QList<OrderData> result;

    QSqlQuery orderQuery;
    orderQuery.prepare("SELECT id, total_price, status, created_at FROM orders WHERE customer_id = ?");
    orderQuery.addBindValue(customerId);

    if (!orderQuery.exec()) {
        qDebug() << "❌ خطا در دریافت سفارشات:" << orderQuery.lastError().text();
        return result;
    }

    while (orderQuery.next()) {
        OrderData order;
        order.orderId = orderQuery.value(0).toInt();
        order.totalPrice = orderQuery.value(1).toDouble();
        order.status = orderQuery.value(2).toString();
        order.createdAt = orderQuery.value(3).toString();

        // حالا آیتم‌های این سفارش
        QSqlQuery itemsQuery;
        itemsQuery.prepare("SELECT restaurant_id, food_name, quantity, unit_price FROM order_items WHERE order_id = ?");
        itemsQuery.addBindValue(order.orderId);

        if (itemsQuery.exec()) {
            while (itemsQuery.next()) {
                TempOrderItem item;
                item.restaurantId = itemsQuery.value(0).toInt();
                item.foodName = itemsQuery.value(1).toString();
                item.quantity = itemsQuery.value(2).toInt();
                item.unitPrice = itemsQuery.value(3).toDouble();
                order.restaurantId = item.restaurantId;
                order.items.append(item);
            }
        }

        result.append(order);
    }

    return result;
}

QList<DatabaseManager::OrderData> DatabaseManager::getOrdersByRestaurantId(int restaurantId)
{
    QList<OrderData> orders;

    QSqlQuery query;
    query.prepare("SELECT o.id, o.customer_id, o.total_price, o.status, o.created_at "
                  "FROM orders o "
                  "JOIN order_items oi ON o.id = oi.order_id "
                  "WHERE oi.restaurant_id = ? "
                  "GROUP BY o.id");
    query.addBindValue(restaurantId);

    if (!query.exec()) {
        qDebug() << "❌ خطا در واکشی سفارش‌های رستوران:" << query.lastError().text();
        return orders;
    }

    while (query.next()) {
        OrderData order;
        order.orderId = query.value(0).toInt();
        order.customerId = query.value(1).toInt();
        order.totalPrice = query.value(2).toDouble();
        order.status = query.value(3).toString();
        order.createdAt = query.value(4).toString();

        // آیتم‌های این سفارش
        QSqlQuery itemQuery;
        itemQuery.prepare("SELECT food_name, quantity, unit_price FROM order_items WHERE order_id = ?");
        itemQuery.addBindValue(order.orderId);
        itemQuery.exec();

        while (itemQuery.next()) {
          TempOrderItem item;
            item.foodName = itemQuery.value(0).toString();
            item.quantity = itemQuery.value(1).toInt();
            item.unitPrice = itemQuery.value(2).toDouble();
            order.items.append(item);
        }

        orders.append(order);
    }

    return orders;
}

QString DatabaseManager::getCustomerNameById(int customerId)
{
    QSqlQuery query;
    query.prepare("SELECT firstname, lastname FROM customers WHERE id = ?");
    query.addBindValue(customerId);

    if (!query.exec() || !query.next()) {
        qDebug() << "❌ خطا در دریافت نام مشتری:" << query.lastError().text();
        return "نام نامشخص";
    }

    QString firstName = query.value(0).toString();
    QString lastName = query.value(1).toString();
    return firstName + " " + lastName;
}


QString DatabaseManager::getCustomerPhoneById(int customerId)
{
    QSqlQuery query;
    query.prepare("SELECT phone FROM customers WHERE id = ?");
    query.addBindValue(customerId);

    if (!query.exec() || !query.next()) {
        qDebug() << "❌ خطا در دریافت شماره مشتری:" << query.lastError().text();
        return "شماره نامشخص";
    }

    return query.value(0).toString();
}

bool DatabaseManager::updateOrderStatus(int orderId, const QString& newStatus)
{
    qDebug() << "Updating orderId:" << orderId << "to status:" << newStatus;

    QSqlQuery query;
    query.prepare("UPDATE orders SET status = ? WHERE id = ?");
    query.addBindValue(newStatus);
    query.addBindValue(orderId);

    if (!query.exec()) {
        qDebug() << "Error updating order status:" << query.lastError().text();
        return false;
    }

    if (query.numRowsAffected() == 0) {
        qDebug() << "No rows were updated. Possible wrong orderId.";
        return false;
    }

    return true;
}


int DatabaseManager::getCustomerIdByOrderId(int orderId)
{
    QSqlQuery query;
    query.prepare("SELECT customer_id FROM orders WHERE id = ?");
    query.addBindValue(orderId);

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return -1; // پیدا نشد
}
DatabaseManager::OrderData DatabaseManager::getLastOrderForCustomer(int customerId)
{
    OrderData order;

    QSqlQuery query;
    query.prepare("SELECT id, restaurant_id, total_price, status, created_at FROM orders WHERE customer_id = ? ORDER BY created_at DESC LIMIT 1");
    query.addBindValue(customerId);

    if (query.exec() && query.next()) {
        order.orderId     = query.value(0).toInt();
        order.restaurantId = query.value(1).toInt();
        order.customerId   = customerId;
        order.totalPrice   = query.value(2).toDouble();
        order.status       = query.value(3).toString();
        order.createdAt    = query.value(4).toString();

        QSqlQuery itemsQuery;
        itemsQuery.prepare("SELECT food_name, quantity, unit_price FROM order_items WHERE order_id = ?");
        itemsQuery.addBindValue(order.orderId);

        if (itemsQuery.exec()) {
            while (itemsQuery.next()) {
                TempOrderItem item;
                item.restaurantId = order.restaurantId;  // چون جدول آیتم‌ها رستوران آیدی ندارن، دستی ستش می‌کنیم
                item.foodName     = itemsQuery.value(0).toString();
                item.quantity     = itemsQuery.value(1).toInt();
                item.unitPrice    = itemsQuery.value(2).toDouble();
                order.items.append(item);
            }
        }
    }

    return order;
}

