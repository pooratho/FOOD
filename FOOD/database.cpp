#include "database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QDebug>

DatabaseManager::DatabaseManager() {
    // اتصال به دیتابیس در سازنده
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("mydatabase.db");
}

DatabaseManager::~DatabaseManager() {
    if (db.isOpen())
        db.close();
}

bool DatabaseManager::connectToDatabase() {
    if (!db.open()) {
        QMessageBox::critical(nullptr, "Database Error", db.lastError().text());
        return false;
    }
    return true;
}

void DatabaseManager::createCustomerTable() {
    QSqlQuery query(db);
    bool success = query.exec(
        "CREATE TABLE IF NOT EXISTS customers ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "firstname TEXT NOT NULL, "
        "lastname TEXT NOT NULL, "
        "phone TEXT NOT NULL, "
        "password TEXT NOT NULL)"
        );

    if (!success) {
        qDebug() << "Create table error:" << query.lastError().text();
    }
}

bool DatabaseManager::insertCustomer(const QString& firstName, const QString& lastName, const QString& phone, const QString& password) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO customers (firstname, lastname, phone, password) "
                  "VALUES (:firstname, :lastname, :phone, :password)");
    query.bindValue(":firstname", firstName);
    query.bindValue(":lastname", lastName);
    query.bindValue(":phone", phone);
    query.bindValue(":password", hashPassword(password));  // هش پسورد

    if (!query.exec()) {
        QMessageBox::warning(nullptr, "Insert Error", query.lastError().text());
        return false;
    }
    return true;
}

QString DatabaseManager::hashPassword(const QString& password) {
    QByteArray hashed = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    return hashed.toHex();
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
