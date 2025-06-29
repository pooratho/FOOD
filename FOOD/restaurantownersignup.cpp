
#include "restaurantownersignup.h"
#include "ui_restaurantownersignup.h"
#include "loginwindow.h"

#include <QScreen>
#include <QGuiApplication>
#include <QTimer>

RestaurantOwnerSignUp::RestaurantOwnerSignUp(LoginWindow *loginWin, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RestaurantOwnerSignUp),
    loginWindow(loginWin)
{
    ui->setupUi(this);

    // ابتدا ارتفاع و عرض فرم را تنظیم می‌کنیم
    this->resize(563, 700); // یا هر ابعاد دلخواه متناسب با مانیتور

    // با تأخیر یک لحظه‌ای فرم را جابجا می‌کنیم
    QTimer::singleShot(0, this, [this]() {
        QScreen *screen = QGuiApplication::primaryScreen();
        QRect screenGeometry = screen->availableGeometry();

        int x = (screenGeometry.width() - this->width()) / 2;
        int y = 30;  // از بالا شروع شود

        this->move(x, y);
    });

    // کدهای اتصال به سرور
    clientSocket = new ClientSocketManager(this);
    clientSocket->connectToServer("127.0.0.1", 1234);

    connect(clientSocket, &ClientSocketManager::messageReceived, this, [=](const QString& response) {
        if (response.startsWith("SIGNUP_OK")) {
            QMessageBox::information(this, "موفق", "ثبت‌نام رستوران با موفقیت انجام شد.");

            RestaurantOwnerMainPage *page = new RestaurantOwnerMainPage();
            page->setAttribute(Qt::WA_DeleteOnClose);         // با بستن آزاد شود
            page->show();                                     // باز کن

            loginWindow->close();  // فقط پنجره اصلی رو ببند
            this->close();

        } else {
            QMessageBox::warning(this, "خطا", "ثبت‌نام رستوران ناموفق بود.");
        }
    });

    provinceCitiesMap["آذربایجان شرقی"] = {"میانه", "اهر", "تبریز", "مرند", "مراغه"};
    provinceCitiesMap["اصفهان"] = {"اصفهان", "کاشان", "خمینی شهر", "شاهین شهر", "نجف آباد"};
    provinceCitiesMap["البرز"] = {"کرج", "فردیس", "کمال شهر", "نظر آباد", "محمد شهر"};
    provinceCitiesMap["تهران"] = {"تهران", "اسلام شهر", "شهریار", "قدس", "ملارد"};
    provinceCitiesMap["خراسان رضوی"] = {"مشهد", "نیشابور", "سبزوار", "تربت حیدریه", "کاشمر"};
    provinceCitiesMap["خوزستان"] = {"اهواز", "دزفول", "آبادان", "ماهشهر", "اندیمشک"};
    provinceCitiesMap["فارس"] = {"شیراز", "مرودشت", "جهرم", "فسا", "کازرون"};
    provinceCitiesMap["کردستان"] = {"سنندج", "سقز", "مریوان", "بانه", "قروه"};
    provinceCitiesMap["گیلان"] = {"رشت", "بندر انزلی", "لاهیجان", "لنگرود", "تالش"};
    provinceCitiesMap["مازندران"] = {"ساری", "بابل", "آمل", "قائم شهر", "بهشهر"};

    // اضافه کردن استان‌ها به comboBox_2 (فرض شده comboBox_2 برای استان‌هاست)
    QStringList provinces = provinceCitiesMap.keys();
    provinces.sort();
    ui->comboBoxProvince->addItems(provinces);

    // انتخاب پیش‌فرض (مثلاً اولین استان)
    if (!provinces.isEmpty())
        on_comboBoxProvince_currentTextChanged(provinces.first());
}

RestaurantOwnerSignUp::~RestaurantOwnerSignUp()
{
    delete ui;
}

void RestaurantOwnerSignUp::on_pushButton_clicked()
{
    QString restaurantName = ui->lineEditRestaurantName->text();
    QString ownerFirstName = ui->lineEditOwnerFirstName->text();
    QString ownerLastName = ui->lineEditOwnerLastName->text();
    QString phone = ui->lineEditPhone->text();
    QString province = ui->comboBoxProvince->currentText();
    QString city = ui->comboBoxCity->currentText();
    QString password = ui->lineEditPassword->text();
    QString passwordRepeat = ui->lineEditPasswordrepeat->text();

    if (password != passwordRepeat) {
        QMessageBox::warning(this, "خطا", "رمز عبور و تکرار آن یکسان نیستند!");
        return;
    }

    if (restaurantName.isEmpty() || ownerFirstName.isEmpty() || ownerLastName.isEmpty() ||
        phone.isEmpty() || province.isEmpty() || city.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "خطا", "لطفاً تمام فیلدها را پر کنید.");
        return;
    }


    // ساخت پیام برای ارسال به سرور
    QString msg = QString("SIGNUP_RESTAURANT:%1:%2:%3:%4:%5:%6:%7")
                      .arg(restaurantName)
                      .arg(ownerFirstName)
                      .arg(ownerLastName)
                      .arg(phone)
                      .arg(province)
                      .arg(city)
                      .arg(password);

    clientSocket->sendMessage(msg);
}

void RestaurantOwnerSignUp::on_comboBoxProvince_currentTextChanged(const QString &province)
{
    qDebug() << "استان انتخاب شده: " << province;

    // گرفتن شهرهای مربوط به استان
    QStringList cities = provinceCitiesMap.value(province);
    cities.sort();

    // پاک کردن شهرهای قبلی
    ui->comboBoxCity->clear();

    // اضافه کردن شهرهای جدید
    ui->comboBoxCity->addItems(cities);
}


void RestaurantOwnerSignUp::on_comboBoxCity_currentTextChanged(const QString &city)
{

}

