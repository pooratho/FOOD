#ifndef DATABASE_H
#define DATABASE_H


#include <QtSql>
#include <QString>


class DatabaseManager {
public:
    DatabaseManager();
    ~DatabaseManager();

    bool signUp(const QString& username, const QString& password);
    bool login(const QString& username, const QString& password);
      QString hashPassword(const QString& password);

private:
    QSqlDatabase db;
    void createUsersTable();
};



#endif // DATABASE_H
