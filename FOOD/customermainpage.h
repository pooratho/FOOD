#ifndef CUSTOMERMAINPAGE_H
#define CUSTOMERMAINPAGE_H

#include <QWidget>
#include <QMap>
#include <QStringList>

#include "customer.h"  // برای استفاده از کلاس Customer

namespace Ui {
class CustomerMainPage;
}

class CustomerMainPage : public QWidget
{
    Q_OBJECT

public:
    explicit CustomerMainPage(Customer* customer, QWidget *parent = nullptr);
    ~CustomerMainPage();

private slots:
    void on_comboBox_2_currentTextChanged(const QString &province);
    void on_comboBox_3_currentTextChanged(const QString &city);

private:
private:
    Ui::CustomerMainPage *ui;
    QMap<QString, QStringList> provinceCitiesMap;
    Customer* customer; // این اضافه بشه

};

#endif // CUSTOMERMAINPAGE_H
