#ifndef ADMIN_H
#define ADMIN_H

#include <QString>

class Admin
{
public:
    Admin(const QString& username, const QString& password);

    QString getUsername() const;
    QString getPassword() const;

private:
    QString username;
    QString password;
};

#endif // ADMIN_H
