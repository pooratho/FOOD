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
        "password TEXT NOT NULL)"
        );

    if (!successCustomer) {
        qDebug() << "Create customers table error:" << query.lastError().text();
    }

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
        "password TEXT NOT NULL)"
        );

    if (!successRestaurant) {
        qDebug() << "Create restaurants table error:" << query.lastError().text();
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


    query.prepare("INSERT INTO customers (firstname, lastname, phone, password) "
                  "VALUES (:firstname, :lastname, :phone, :password)");
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
                                       const QString& password) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO restaurants (restaurant_name, owner_firstname, owner_lastname, phone, province, city, password) "
                  "VALUES (:restaurant_name, :owner_firstname, :owner_lastname, :phone, :province, :city, :password)");
    query.bindValue(":restaurant_name", restaurantName);
    query.bindValue(":owner_firstname", ownerFirstName);
    query.bindValue(":owner_lastname", ownerLastName);
    query.bindValue(":phone", phone);
    query.bindValue(":province", province);
    query.bindValue(":city", city);
    query.bindValue(":password", hashPassword(password));

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

    // بررسی مشتری
    query.prepare("SELECT COUNT(*) FROM customers WHERE firstname = :first AND lastname = :last AND password = :pass");
    query.bindValue(":first", firstName);
    query.bindValue(":last", lastName);
    query.bindValue(":pass", hashed);
    if (query.exec() && query.next() && query.value(0).toInt() > 0) {
        return UserRole::Customer;
    }

    // بررسی رستوران‌دار
    query.prepare("SELECT COUNT(*) FROM restaurants WHERE owner_firstname = :first AND owner_lastname = :last AND password = :pass");
    query.bindValue(":first", firstName);
    query.bindValue(":last", lastName);
    query.bindValue(":pass", hashed);
    if (query.exec() && query.next() && query.value(0).toInt() > 0) {
        return UserRole::Restaurant;
    }

    // بررسی مدیر
    // query.prepare("SELECT COUNT(*) FROM admins WHERE firstname = :first AND lastname = :last AND password = :pass");
    // query.bindValue(":first", firstName);
    // query.bindValue(":last", lastName);
    // query.bindValue(":pass", hashed);
    // if (query.exec() && query.next() && query.value(0).toInt() > 0) {
    //     return UserRole::Admin;
    // }

    return UserRole::None;
}


// void DatabaseManager::createUsersTable() {
//     QSqlQuery query;
//     QString createTable = "CREATE TABLE IF NOT EXISTS users ("
//                           "id INTEGER PRIMARY KEY AUTOINCREMENT, "
//                           "username TEXT UNIQUE NOT NULL, "
//                           "password TEXT NOT NULL)";
//     if (!query.exec(createTable)) {
//         qDebug() << "Failed to create users table:" << query.lastError();
//     }
// }

// bool DatabaseManager::signUp(const QString& username, const QString& password, const QString& role) {
//     QString hashedPassword = hashPassword(password);

//     QSqlQuery query;
//     query.prepare("INSERT INTO users (username, password, role) VALUES (?, ?, ?)");
//     query.addBindValue(username);
//     query.addBindValue(hashedPassword);
//     query.addBindValue(role);

//     if (!query.exec()) {
//         qDebug() << "SignUp failed:" << query.lastError();
//         return false;
//     }
//     return true;
// }


// bool DatabaseManager::login(const QString& username, const QString& password, const QString& role) {
//     QString hashedPassword = hashPassword(password);

//     QSqlQuery query;
//     query.prepare("SELECT COUNT(*) FROM users WHERE username = ? AND password = ? AND role = ?");
//     query.addBindValue(username);
//     query.addBindValue(hashedPassword);
//     query.addBindValue(role);

//     if (!query.exec()) {
//         qDebug() << "Login query failed:" << query.lastError();
//         return false;
//     }

//     if (query.next()) {
//         int count = query.value(0).toInt();
//         return count == 1;
//     }

//     return false;
// }
