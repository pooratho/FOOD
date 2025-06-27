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

class DatabaseManager {
public:
    DatabaseManager();
    ~DatabaseManager();

      void createUsersTable();
    bool signUp(const QString& username, const QString& password, const QString& role) ;
    bool login(const QString& username, const QString& password, const QString& role);
      QString hashPassword(const QString& password);
    bool connectToDatabase();
    void createCustomerTable();
    bool insertCustomer(const QString& firstName, const QString& lastName, const QString& phone, const QString& password);


private:
    QSqlDatabase db;


};



#endif // DATABASE_H
