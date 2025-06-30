#include "restaurantownersignup.h"
#include "ui_restaurantownersignup.h"
#include "loginwindow.h"
#include "restaurant.h"
#include "restaurantowner.h"
#include "restaurantownermainpage.h"

#include <QScreen>
#include <QGuiApplication>
#include <QTimer>
#include <QMessageBox>

RestaurantOwnerSignUp::RestaurantOwnerSignUp(LoginWindow *loginWin, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RestaurantOwnerSignUp),
    loginWindow(loginWin)
{
    ui->setupUi(this);

    this->resize(563, 700);

    QTimer::singleShot(0, this, [this]() {
        QScreen *screen = QGuiApplication::primaryScreen();
        QRect screenGeometry = screen->availableGeometry();

        int x = (screenGeometry.width() - this->width()) / 2;
        int y = 30;
        this->move(x, y);
    });

    clientSocket = new ClientSocketManager(this);
    clientSocket->connectToServer("127.0.0.1", 1234);

    connect(clientSocket, &ClientSocketManager::messageReceived, this, [=](const QString &msg) {
        if (msg.startsWith("SIGNUP_OK")) {
            QMessageBox::information(this, "موفقیت", "ثبت نام با موفقیت انجام شد.");

            // ساخت شی Restaurant
            Restaurant restaurant(
                0,  // ID موقت
                cachedRestaurantName,
                cachedOwnerFirstName + cachedOwnerLastName,  // owner username فرضی
                cachedProvince,
                cachedCity,
                true  // isActive
                );

            // ساخت شی RestaurantOwner
            RestaurantOwner* owner = new RestaurantOwner(
                cachedOwnerFirstName,
                cachedOwnerLastName,
                cachedPhone,
                cachedPassword,
                restaurant
                );

            // باز کردن صفحه اصلی رستوران‌دار
            RestaurantOwnerMainPage* page = new RestaurantOwnerMainPage(owner);
            page->setAttribute(Qt::WA_DeleteOnClose);
            page->show();

           // loginWindow->close();
            this->close();
        } else if (msg.startsWith("SIGNUP_FAIL")) {
            QMessageBox::warning(this, "خطا", "ثبت نام ناموفق بود، دوباره تلاش کنید.");
        }
    });

    // لیست استان‌ها و شهرها
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

    QStringList provinces = provinceCitiesMap.keys();
    provinces.sort();
    ui->comboBoxProvince->addItems(provinces);

    if (!provinces.isEmpty())
        on_comboBoxProvince_currentTextChanged(provinces.first());
}

RestaurantOwnerSignUp::~RestaurantOwnerSignUp()
{
    delete ui;
}

void RestaurantOwnerSignUp::on_pushButton_clicked()
{
    QString restaurantName = ui->lineEditRestaurantName->text().trimmed();
    QString ownerFirstName = ui->lineEditOwnerFirstName->text().trimmed();
    QString ownerLastName = ui->lineEditOwnerLastName->text().trimmed();
    QString phone = ui->lineEditPhone->text().trimmed();
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

    // ذخیره اطلاعات برای بعد از پاسخ مثبت سرور
    cachedRestaurantName = restaurantName;
    cachedOwnerFirstName = ownerFirstName;
    cachedOwnerLastName = ownerLastName;
    cachedPhone = phone;
    cachedProvince = province;
    cachedCity = city;
    cachedPassword = password;

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
    QStringList cities = provinceCitiesMap.value(province);
    cities.sort();
    ui->comboBoxCity->clear();
    ui->comboBoxCity->addItems(cities);
}

void RestaurantOwnerSignUp::on_comboBoxCity_currentTextChanged(const QString &city)
{
    Q_UNUSED(city);
    // اگر نیاز به پردازش بود، اضافه می‌کنیم
}
