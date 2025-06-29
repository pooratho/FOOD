#include "user.h"

User::User(const QString& firstName,
           const QString& lastName,
           const QString& phone,
           const QString& password)
    : firstName(firstName), lastName(lastName), phone(phone), password(password) {}

User::~User() {}

QString User::getFirstName() const {
    return firstName;
}

QString User::getLastName() const {
    return lastName;
}

QString User::getPhone() const {
    return phone;
}

QString User::getPassword() const {
    return password;
}

void User::setPassword(const QString& newPassword) {
    password = newPassword;
}
