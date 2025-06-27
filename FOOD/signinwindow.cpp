#include "signinwindow.h"
#include "ui_signIn.h"  // نام فایل هدر تولید شده توسط uic دقیقاً این است

SignInWindow::SignInWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
}

SignInWindow::~SignInWindow()
{
    delete ui;
}

void SignInWindow::on_pushButton_clicked()
{

}

