#include "loginwindow.h"
#include "ui_login.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::Form)
{
    ui->setupUi(this);

    // مثال: اتصال دکمه‌ها به slotها (اگه بعداً بخوای اضافه کنی)
    // connect(ui->pushButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}





