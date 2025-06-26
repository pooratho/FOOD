#include "customersignupwindow.h"
#include "ui_customersignup.h"

CustomerSignUpWindow::CustomerSignUpWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
}

CustomerSignUpWindow::~CustomerSignUpWindow()
{
    delete ui;
}
