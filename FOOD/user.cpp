#include "user.h"

User::User(const QString& firstName,
           const QString& lastName,
           const QString& phone,
           const QString& password,
           bool isBlocked)
    : firstName(firstName),
    lastName(lastName),
    phone(phone),
    password(password),
    isBlocked(isBlocked) {}

User::~User() {}

QString User::getFirstName() const { return firstName; }
QString User::getLastName() const { return lastName; }
QString User::getPhone() const { return phone; }
QString User::getPassword() const { return password; }

bool User::getIsBlocked() const { return isBlocked; }
void User::setIsBlocked(bool blocked) { isBlocked = blocked; }

void User::setPassword(const QString& newPassword) {
    password = newPassword;
}
