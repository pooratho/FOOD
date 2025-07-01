#ifndef USER_H
#define USER_H

#include <QString>

class User {
protected:
    QString firstName;
    QString lastName;
    QString phone;
    QString password;
    bool isBlocked;

public:
    User(const QString& firstName,
         const QString& lastName,
         const QString& phone,
         const QString& password,
         bool isBlocked = false);

    virtual ~User();

    virtual QString getRole() const = 0;

    QString getFirstName() const;
    QString getLastName() const;
    QString getPhone() const;
    QString getPassword() const;
    bool getIsBlocked() const;
    void setIsBlocked(bool blocked);

    void setPassword(const QString& newPassword);
};

#endif // USER_H
