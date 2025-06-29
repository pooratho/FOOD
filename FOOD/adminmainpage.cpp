#include "adminmainpage.h"
#include "ui_adminmainpage.h"

AdminMainPage::AdminMainPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::adminMainPage)
{
    ui->setupUi(this);

    // تنظیمات ظاهری یا رویدادها
    setWindowTitle("پنل مدیریت سیستم");

}

AdminMainPage::~AdminMainPage()
{
    delete ui;
}
