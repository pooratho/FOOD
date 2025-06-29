#include "food.h"

Food::Food() : price(0.0) {}

Food::Food(const QString &name, const QString &description, double price, const QString &category)
    : name(name), description(description), price(price), category(category) {}

QString Food::getName() const {
    return name;
}

QString Food::getDescription() const {
    return description;
}

double Food::getPrice() const {
    return price;
}

QString Food::getCategory() const {
    return category;
}

void Food::setName(const QString &name) {
    this->name = name;
}

void Food::setDescription(const QString &description) {
    this->description = description;
}

void Food::setPrice(double price) {
    this->price = price;
}

void Food::setCategory(const QString &category) {
    this->category = category;
}
