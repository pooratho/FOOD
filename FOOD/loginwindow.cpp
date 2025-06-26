#include "loginwindow.h"
#include "ui_login.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}
