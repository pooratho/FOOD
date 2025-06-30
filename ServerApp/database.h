#ifndef DATABASE_H
#define DATABASE_H


#include <QtSql>
#include <QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QString>
#include <QVariantList>
#include <QMessageBox>
#include "admin.h"

class DatabaseManager {
public:
    DatabaseManager();
    ~DatabaseManager();

    void createUsersTable();
    bool signUp(const QString& username, const QString& password, const QString& role) ;
    bool login(const QString& username, const QString& password, const QString& role);
    QString hashPassword(const QString& password);
    bool connectToDatabase();
    void createTables();
    bool insertCustomer(const QString& firstName, const QString& lastName, const QString& phone, const QString& password);
    bool insertRestaurant(const QString& restaurantName,
                          const QString& ownerFirstName,
                          const QString& ownerLastName,
                          const QString& phone,
                          const QString& province,
                          const QString& city,
                          const QString& password);

    enum class UserRole {
        None,
        Customer,
        Restaurant,
        Admin
    };

    UserRole checkUserLogin(const QString& firstName, const QString& lastName, const QString& password);


private:
    QSqlDatabase db;

    QVector<Admin> adminList;


};



#endif // DATABASE_H
