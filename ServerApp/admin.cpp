#include "admin.h"

Admin::Admin(const QString& username, const QString& password)
    : username(username), password(password) {}

QString Admin::getUsername() const {
    return username;
}

QString Admin::getPassword() const {
    return password;
}
