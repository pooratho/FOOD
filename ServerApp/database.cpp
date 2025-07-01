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



