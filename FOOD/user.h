#ifndef USER_H
#define USER_H

#include <QString>

class User {
protected:
    QString firstName;
    QString lastName;
    QString phone;
    QString password;

public:
    User(const QString& firstName,
         const QString& lastName,
         const QString& phone,
         const QString& password);

    virtual ~User(); // مهم برای پلی‌مورفیسم

    virtual QString getRole() const = 0; // کلاس abstract

    QString getFirstName() const;
    QString getLastName() const;
    QString getPhone() const;
    QString getPassword() const;

    void setPassword(const QString& newPassword);
};

#endif // USER_H
