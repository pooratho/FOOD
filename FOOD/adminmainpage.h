#ifndef ADMINMAINPAGE_H
#define ADMINMAINPAGE_H

#include <QWidget>

namespace Ui {
class AdminMainPage;
}

class AdminMainPage : public QWidget
{
    Q_OBJECT

public:
    explicit AdminMainPage(QWidget *parent = nullptr);
    ~AdminMainPage();

private:
    Ui::AdminMainPage *ui;
};

#endif // ADMINMAINPAGE_H
