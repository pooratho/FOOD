#include "customermainpage.h"
#include "qtimer.h"
#include "ui_customermainpage.h"
#include "restaurantmenu.h"
#include "shoppingcartitemwidget.h"

#include <QDebug>
#include<QMessageBox>

CustomerMainPage::CustomerMainPage(Customer* customer, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CustomerMainPage)
    , customer(customer)
{
    ui->setupUi(this);

    connect(ui->tableWidget, &QTableWidget::cellDoubleClicked,
            this, &CustomerMainPage::onTableItemDoubleClicked);

    // مقداردهی به clientSocket
    clientSocket = new ClientSocketManager(this);
    clientSocket->connectToServer("127.0.0.1", 1234);

    // اتصال سیگنال دریافت پیام
    connect(clientSocket, &ClientSocketManager::messageReceived,
            this, &CustomerMainPage::handleServerMessage);

    //تغییر
    connect(clientSocket, &ClientSocketManager::connected, this, [=]() {
        qDebug() << "✅ اتصال به سرور برقرار شد";

        // 🔻🔻 ثبت لاگین مشتری در سرور
        QString loginMsg = QString("LOGIN:Customer:%1:%2:%3")
                               .arg(customer->getFirstName())
                               .arg(customer->getLastName())
                               .arg(customer->getPassword());
        clientSocket->sendMessage(loginMsg);

        // درخواست‌های بعدی
        clientSocket->sendMessage("GET_RESTAURANTS");

        QTimer::singleShot(100, this, [=]() {
            QString msg = "GET_CART:" + customer->getPhone();
            clientSocket->sendMessage(msg);
        });
    });




    connect(ui->comboBoxProvince, &QComboBox::currentTextChanged,
            this, &CustomerMainPage::sendFilteredRequest);

    connect(ui->comboBoxCity, &QComboBox::currentTextChanged,
            this, &CustomerMainPage::sendFilteredRequest);

    connect(ui->comboBoxFoodType, &QComboBox::currentTextChanged,
            this, &CustomerMainPage::sendFilteredRequest);

    connect(ui->comboBoxProvince, &QComboBox::currentTextChanged,
            this, &CustomerMainPage::on_comboBox_2_currentTextChanged);

    // نقشه استان‌ها
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
        on_comboBox_2_currentTextChanged(provinces.first());

    ui->tabWidget->setCurrentIndex(0);
}

CustomerMainPage::~CustomerMainPage()
{
    delete ui;
}

void CustomerMainPage::on_comboBox_2_currentTextChanged(const QString &province)
{
    qDebug() << "استان انتخاب شده: " << province;

    QStringList cities = provinceCitiesMap.value(province);
    cities.sort();

    ui->comboBoxCity->clear();
    ui->comboBoxCity->addItems(cities);
}

void CustomerMainPage::on_comboBox_3_currentTextChanged(const QString &city)
{
    qDebug() << "شهر انتخاب شده: " << city;
}

void CustomerMainPage::handleServerMessage(const QString& msg)
{
    qDebug() << "پیام از سرور: " << msg;

    if (msg.startsWith("RESTAURANT_LIST:")) {
        QString data = msg.mid(QString("RESTAURANT_LIST:").length());
        QStringList rows = data.split(";", Qt::SkipEmptyParts);

        // جدول رو پاک می‌کنیم
        ui->tableWidget->clearContents();

        ui->tableWidget->setRowCount(rows.size());

        for (int i = 0; i < rows.size(); ++i) {
            QStringList columns = rows[i].split("|");
            if (columns.size() != 3)
                continue;

            for (int j = 0; j < 3; ++j) {
                QTableWidgetItem *item = new QTableWidgetItem(columns[j]);
                item->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(i, j, item);
            }
        }

        // اگر هیچ رستوانی نبود
        if (rows.isEmpty()) {
            QMessageBox::information(this, "اطلاع", "هیچ رستورانی در سیستم ثبت نشده است.");
        }
        ui->tableWidget->setColumnWidth(0, 360); // ستون "نام رستوران"
        ui->tableWidget->setColumnWidth(1, 200); // ستون "نوع غذا"
        ui->tableWidget->setColumnWidth(2, 370); // ستون "آدرس"

        ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    }
    else if (msg.startsWith("RESTAURANT_LIST_FAIL")) {
        QMessageBox::warning(this, "خطا", "دریافت لیست رستوران‌ها ناموفق بود!");
    }

    else if (msg.startsWith("GET_CART_OK:")) {
        QString data = msg.mid(QString("GET_CART_OK:").length());
        QStringList items = data.split("#", Qt::SkipEmptyParts);

        for (const QString& itemStr : items) {
            QStringList parts = itemStr.split("|");
            if (parts.size() == 4) {
                QString rest = parts[0];
                QString food = parts[1];
                int qty = parts[2].toInt();
                double price = parts[3].toDouble();

                CartItem* item = new CartItem(food, rest, qty, price);
                cartItems.append(item);
            }
        }

        updateCartDisplay();
    }
    else if (msg.startsWith("UPDATE_CART_OK:")) {
        qDebug() << "✅ به‌روزرسانی سبد خرید با موفقیت انجام شد.";
    }
    else if (msg.startsWith("UPDATE_CART_FAIL:")) {
        QString reason = msg.mid(QString("UPDATE_CART_FAIL:").length());
        QMessageBox::warning(this, "خطا در به‌روزرسانی سبد خرید", reason);
    }
    else if (msg.startsWith("ORDER_ITEM:")) {
        handleIncomingOrderItem(msg);
    }
    //
    else if (msg.startsWith("ORDER_STATUS_UPDATED:")) {
        QString data = msg.mid(QString("ORDER_STATUS_UPDATED:").length());
        QStringList parts = data.split("|");
        if (parts.size() == 2) {
            int orderId = parts[0].toInt();
            QString newStatus = parts[1];
            showOrderStatusNotification(orderId, newStatus); // نمایش نوتیف
            qDebug() << "✅ پیام نوتیف پردازش شد برای سفارش:" << orderId << "و وضعیت:" << newStatus;
        }
    }



    else {
        qDebug() << "پیام ناشناخته از سرور دریافت شد.";
    }

}


void CustomerMainPage::sendFilteredRequest()
{
    QString province = ui->comboBoxProvince->currentText().trimmed();
    QString city = ui->comboBoxCity->currentText().trimmed();
    QString type = ui->comboBoxFoodType->currentText().trimmed();

    // بررسی اینکه آیا مقدار نوع غذا واقعی است
    if (type == "نوع رستوران" || type == "همه" || type.isEmpty())
        type = ""; // از فیلتر حذفش کن

    QStringList filters;

    if (!province.isEmpty())
        filters << "province=" + province;
    if (!city.isEmpty())
        filters << "city=" + city;
    if (!type.isEmpty())
        filters << "type=" + type;

    // اگر هیچ فیلتری فعال نیست، درخواست همه رستوران‌ها را بفرست
    if (filters.isEmpty()) {
        clientSocket->sendMessage("GET_RESTAURANTS");
        return;
    }

    QString msg = "GET_RESTAURANTS_FILTERED:" + filters.join(";");
    qDebug() << "🔍 پيام ارسال‌شده به سرور: " << msg;
    clientSocket->sendMessage(msg);
}

void CustomerMainPage::onTableItemDoubleClicked(int row, int)
{
    QString restaurantName = ui->tableWidget->item(row, 0)->text();
    QString foodType = ui->tableWidget->item(row, 1)->text();
    QString address = ui->tableWidget->item(row, 2)->text();

    qDebug() << "رستوران انتخاب‌شده:" << restaurantName;

    QString customerPhone =customer->getPhone() + "#";   // شماره تلفن مشتری

    restaurantmenu* menuPage = new restaurantmenu(restaurantName, customerPhone, nullptr);
    menuPage->setWindowFlag(Qt::Window);
    menuPage->setAttribute(Qt::WA_DeleteOnClose);  // تنظیم قبل از نمایش
    menuPage->show();

    connect(menuPage, &restaurantmenu::cartItemsReady, this, [=](const QList<CartItem>& newItems) {
        for (const CartItem& item : newItems) {
            if (isFoodInCart(item.getRestaurantName(), item.getFoodName())) {
                QMessageBox::warning(this, "تکراری",
                                     QString("غذای %1 قبلاً به سبد خرید اضافه شده است.").arg(item.getFoodName()));
                continue;
            }

            cartItems.append(new CartItem(item));

            QString message = "ADD_TO_CART:" + customer->getPhone() + "#" +
                              item.getRestaurantName() + "|" +
                              item.getFoodName() + "|" +
                              QString::number(item.getQuantity()) + "|" +
                              QString::number(item.getUnitPrice());

            clientSocket->sendMessage(message);
        }

        qDebug() << "📦 سبد خرید آپدیت شد. تعداد آیتم‌ها:" << cartItems.size();
        updateCartDisplay();
    });

}

void CustomerMainPage::updateCartDisplay()
{
    ui->listWidget_2->clear();  // این لیست‌ویجت در تب دوم باید تعریف شده باشه

    if (cartItems.isEmpty()) {
        ui->label_4->show();   // نمایش پیام خالی بودن
    } else {
        ui->label_4->hide();   // مخفی کردن پیام
    }

    int totalPrice = 0;

    for (CartItem* item : cartItems) {
        auto* listItem = new QListWidgetItem(ui->listWidget_2);
        auto* itemWidget = new ShoppingCartItemWidget(*item);  // کپی از CartItem

        listItem->setSizeHint(QSize(930, 73));  // عرض: 500، ارتفاع: 120 (به دلخواه خودت)
        ui->listWidget_2->addItem(listItem);
        ui->listWidget_2->setItemWidget(listItem, itemWidget);

        // اتصال برای حذف آیتم
        connect(itemWidget, &ShoppingCartItemWidget::removeClicked, this, [=](ShoppingCartItemWidget* widget){
            removeCartItem(widget);  // تابعش رو پایین تعریف می‌کنیم
        });

        connect(itemWidget, &ShoppingCartItemWidget::quantityChanged, this, [=](int newQty){
            item->setQuantity(newQty);
            updateTotalPriceDisplay();

            QString phone = customer->getPhone().trimmed(); // شماره موبایل
            QString rest = item->getRestaurantName().trimmed();
            QString food = item->getFoodName().trimmed();

            qDebug() << "📞 شماره موبایل در پیام:" << phone;

            QString message = "UPDATE_CART:" + phone + "#" + rest + "|" + food + "|" + QString::number(newQty);

            qDebug() << "⬆️ پیام UPDATE_CART: " << message;
            clientSocket->sendMessage(message);
        });



        totalPrice += item->getQuantity() * item->getUnitPrice();
    }

    ui->label_7->setText("  " + QString::number(totalPrice) + " تومان");

}

void CustomerMainPage::removeCartItem(ShoppingCartItemWidget* widget)
{
    for (int i = 0; i < ui->listWidget_2->count(); ++i) {
        auto* listItem = ui->listWidget_2->item(i);
        if (ui->listWidget_2->itemWidget(listItem) == widget) {

            // حذف از cartItems
            for (int j = 0; j < cartItems.size(); ++j) {
                if (cartItems[j]->getFoodName() == widget->getCartItem().getFoodName()
                    && cartItems[j]->getRestaurantName() == widget->getCartItem().getRestaurantName()) {
                    QString phone = customer->getPhone().trimmed();
                    QString rest = widget->getCartItem().getRestaurantName().trimmed();
                    QString food = widget->getCartItem().getFoodName().trimmed();
                    qDebug() << "شماره موبایل در پیام:" << customer->getPhone();

                    QString message = "REMOVE_FROM_CART:" + phone + "#" + rest + "|" + food;
                    qDebug() << "❌ پیام REMOVE_FROM_CART: " << message;
                    clientSocket->sendMessage(message);


                    delete cartItems[j];
                    cartItems.removeAt(j);
                    break;
                }
            }

            delete widget;
            delete listItem;
            break;
        }
    }
    updateTotalPriceDisplay();
}

void CustomerMainPage::updateTotalPriceDisplay()
{
    int totalPrice = 0;
    for (CartItem* item : cartItems) {
        totalPrice += item->getQuantity() * item->getUnitPrice();
    }
    ui->label_7->setText("  " + QString::number(totalPrice) + " تومان");
}

bool CustomerMainPage::isFoodInCart(const QString& restaurantName, const QString& foodName) const {
    for (const CartItem* existing : cartItems) {
        if (existing->getFoodName() == foodName && existing->getRestaurantName() == restaurantName) {
            return true;
        }
    }
    return false;
}

void CustomerMainPage::on_pushButton_clicked()
{ if (cartItems.isEmpty()) {
        QMessageBox::information(this, "سبد خرید خالی", "هیچ آیتمی در سبد خرید وجود ندارد.");
        return;
    }

    // مرحله ۱: دسته‌بندی بر اساس رستوران
    QMap<QString, QVector<CartItem>> restaurantOrders;

    for (CartItem* item : cartItems) {
        restaurantOrders[item->getRestaurantName()].append(*item);  // کپی از آیتم
    }

    QString phone = customer->getPhone().trimmed();

    // مرحله ۲: ساخت سفارش برای هر رستوران و ارسال به سرور
    for (auto it = restaurantOrders.begin(); it != restaurantOrders.end(); ++it) {
        QString restaurantName = it.key();
        QVector<CartItem> items = it.value();

        int fakeOrderId = QDateTime::currentSecsSinceEpoch();  // ID موقت
        Order order(fakeOrderId, phone, items);

        // مرحله ۳: ساخت پیام
        QString message = "SUBMIT_ORDER:" + phone + "#" + restaurantName;

        for (const CartItem& item : order.getItems()) {
            message += "|" + item.getFoodName() + "," +
                       QString::number(item.getQuantity()) + "," +
                       QString::number(item.getUnitPrice());
        }

        qDebug() << "📤 ارسال سفارش به سرور: " << message;
        clientSocket->sendMessage(message);
    }

    QMessageBox::information(this, "ثبت سفارش", "سفارش شما با موفقیت ثبت شد.");
    cartItems.clear();
    updateCartDisplay();
}

void CustomerMainPage::on_tabWidget_currentChanged(int index)
{
    if (index == 2) {  // فرض: تب ۲ مربوط به "وضعیت سفارش"
        refreshOrders();
    }
}
void CustomerMainPage::handleIncomingOrderItem(const QString& msg)
{
    QString data = msg.mid(QString("ORDER_ITEM:").length()).trimmed();
    QStringList parts = data.split("#");
    if (parts.size() != 2) {
        qWarning() << "⚠️ پیام ORDER_ITEM فرمت درستی ندارد:" << msg;
        return;
    }


    QString restaurantName = parts[0];
    QStringList orderDataParts = parts[1].split("|");
    if (orderDataParts.size() < 4) {
        qWarning() << "⚠️ اطلاعات سفارش ناقص است:" << orderDataParts;
        return;
    }


    int orderId = orderDataParts[0].toInt();

    // ❗ بررسی اینکه این سفارش قبلاً نمایش داده شده یا نه
    if (shownOrderIds.contains(orderId)) {
        qDebug() << "ℹ️ سفارش تکراری نادیده گرفته شد:" << orderId;
        return;
    }
    shownOrderIds.insert(orderId);  // اضافه کن که دیگه نشون داده نشه

    double totalPrice = orderDataParts[1].toDouble();
    QString status = orderDataParts[2];
    QString dateStr = orderDataParts[3];

    QStringList foodList = orderDataParts.mid(4);
    QString foodText;
    for (const QString& foodRaw : foodList) {
        QStringList foodParts = foodRaw.split(",");
        if (foodParts.size() != 3) continue;
        QString name = foodParts[0];
        int qty = foodParts[1].toInt();
        double price = foodParts[2].toDouble();

        foodText += "  " + name + " × " + QString::number(qty) +
                    " - " + QString::number(price) + " تومان\n";
    }

    orderitemwidget* widget = new orderitemwidget(this);
    widget->setOrderId(orderId);
    widget->setRestaurantName(restaurantName);
    widget->setFoodListText(foodText);
    widget->setStatus(status);

    QListWidgetItem* listItem = new QListWidgetItem();
    listItem->setSizeHint(QSize(890, 120));
    ui->orderListWidget->addItem(listItem);
    ui->orderListWidget->setItemWidget(listItem, widget);

    ui->label_5->setVisible(ui->orderListWidget->count() == 0);
}


void CustomerMainPage::on_pushButton_2_clicked()
{

   refreshOrders();
}

void CustomerMainPage::clearOrderWidgets()
{
    while (ui->orderListWidget->count() > 0) {
        QListWidgetItem* item = ui->orderListWidget->takeItem(0);
        delete ui->orderListWidget->itemWidget(item); // پاک کردن ویجت
        delete item; // پاک کردن آیتم
    }
}

void CustomerMainPage::refreshOrders()
{
    clearOrderWidgets();
     shownOrderIds.clear();
    QString msg = "GET_CUSTOMER_ORDERS:" + customer->getPhone();
    clientSocket->sendMessage(msg);
}


void CustomerMainPage::showOrderStatusNotification(int orderId, const QString& newStatus)
{
    qDebug() << "🔔 نمایش نوتیف سفارش:" << orderId << "و وضعیت:" << newStatus;
    QMessageBox::information(this, "به‌روزرسانی سفارش",
                             QString("وضعیت سفارش شماره %1 تغییر کرد به:\n%2").arg(orderId).arg(newStatus));
}

