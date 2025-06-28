#ifndef CUSTOMERMAINPAGE_H
#define CUSTOMERMAINPAGE_H

#include <QWidget>
#include <QMap>
#include <QStringList>

#include "ui_customerMainPage.h"

namespace Ui {
class customerMainPage;
}

class CustomerMainPage : public QWidget
{
    Q_OBJECT

public:
    explicit CustomerMainPage(QWidget *parent = nullptr);
    ~CustomerMainPage();

private slots:
    void on_comboBox_2_currentTextChanged(const QString &province);
    void on_comboBox_3_currentTextChanged(const QString &city);

private:
    Ui::customerMainPage *ui;
    QMap<QString, QStringList> provinceCitiesMap;
};

#endif // CUSTOMERMAINPAGE_H
