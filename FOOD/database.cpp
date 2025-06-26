#include "database.h"
#include <QDebug>
#include <QCryptographicHash> // این بالای فایل cpp باشه

QString DatabaseManager::hashPassword(const QString& password) {
    QByteArray hashed = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    return hashed.toHex();
}

DatabaseManager::DatabaseManager() {
    db = QSqlDatabase::addDatabase("QSQLITE"); // از SQLite استفاده می‌کنیم
    db.setDatabaseName("food_ordering.db");

    if (!db.open()) {
        qDebug() << "Error: database not opened!";
    } else {
        qDebug() << "Database opened successfully";
        createUsersTable();
    }
}

DatabaseManager::~DatabaseManager() {
    if (db.isOpen())
        db.close();
}

void DatabaseManager::createUsersTable() {
    QSqlQuery query;
    QString createTable = "CREATE TABLE IF NOT EXISTS users ("
                          "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                          "username TEXT UNIQUE NOT NULL, "
                          "password TEXT NOT NULL)";
    if (!query.exec(createTable)) {
        qDebug() << "Failed to create users table:" << query.lastError();
    }
}

bool DatabaseManager::signUp(const QString& username, const QString& password) {
    QString hashedPassword = hashPassword(password);  // هش کردن

    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password) VALUES (?, ?)");
    query.addBindValue(username);
    query.addBindValue(hashedPassword);

    if (!query.exec()) {
        qDebug() << "SignUp failed:" << query.lastError();
        return false;
    }
    return true;
}


bool DatabaseManager::login(const QString& username, const QString& password) {
    QString hashedPassword = hashPassword(password);  // هش کردن پسورد ورودی

    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE username = ?");
    query.addBindValue(username);

    if (!query.exec()) {
        qDebug() << "Login query failed:" << query.lastError();
        return false;
    }

    if (query.next()) {
        QString storedPassword = query.value(0).toString();
        return storedPassword == hashedPassword;
    }
    return false;
}

