#ifndef ADMINMAINPAGE_H
#define ADMINMAINPAGE_H

#include <QWidget>

namespace Ui {
class adminMainPage;
}

class AdminMainPage : public QWidget
{
    Q_OBJECT

public:
    explicit AdminMainPage(QWidget *parent = nullptr);
    ~AdminMainPage();

private:
    Ui::adminMainPage *ui;
};

#endif // ADMINMAINPAGE_H
