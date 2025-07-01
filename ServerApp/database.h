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
                                           const QString& password,
                          const QString& restaurantType);

    enum class UserRole {
        None,
        Customer,
        Restaurant,
        Admin
    };

    struct CartItem {
        QString foodName;
        QString restaurantName;
        int quantity;
        double unitPrice;

        CartItem(const QString& f, const QString& r, int q, double u)
            : foodName(f), restaurantName(r), quantity(q), unitPrice(u) {}
    };
    bool addOrUpdateCartItemByPhone(const QString& phone,
                                    const QString& restaurantName,
                                    const QString& foodName,
                                    int quantity,
                                    double unitPrice);

    bool removeCartItemByPhone(const QString& phone,
                               const QString& restaurantName,
                               const QString& foodName);

    QList<CartItem> getCartItemsByPhone(const QString& phone);

    UserRole checkUserLogin(const QString& firstName, const QString& lastName, const QString& password);
    int getRestaurantId(const QString& firstName, const QString& lastName, const QString& password);
      int getRestaurantIdByRestaurantName(const QString& name);
    QString getRestaurantNameById(int restaurantId);
      bool deleteFood(const QString& category, const QString& name);

bool addOrUpdateCartItem(int customerId,
                                          const QString& restaurantName,
                                          const QString& foodName,
                                          int quantity,
                               double unitPrice) ;


bool removeCartItem(int customerId,
                                     const QString& restaurantName,
                    const QString& foodName);
QList<DatabaseManager::CartItem> getCartItemsByCustomerId(int customerId) ;

QString getPhoneByName(const QString& firstName, const QString& lastName);
private:
    QSqlDatabase db;

    QVector<Admin> adminList;


};



#endif // DATABASE_H
