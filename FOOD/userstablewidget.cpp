#include "userstablewidget.h"
#include "ui_userstablewidget.h"
#include "clientsocketmanager.h"
#include <QMessageBox>

UserTableWidget::UserTableWidget(ClientSocketManager* socketManager, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::usertablewidget)
    , clientSocket(socketManager)
{
    ui->setupUi(this);

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->tableWidget->setColumnWidth(0, 150);
    ui->tableWidget->setColumnWidth(1, 150);
    ui->tableWidget->setColumnWidth(2, 150);
    ui->tableWidget->setColumnWidth(3, 137);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    // اتصال دکمه تغییر وضعیت به تابع
    connect(ui->pushButton_2, &QPushButton::clicked, this, &UserTableWidget::on_pushButton_2_clicked);
}
UserTableWidget::~UserTableWidget()
{
    delete ui;
}
QString normalizePhoneNumber(const QString& phone) {
    QString p = phone.trimmed();

    // اگر با +98 شروع نشده و با 0 شروع شده
    if (!p.startsWith("+98") && p.startsWith("0")) {
        p = "+98" + p.mid(1);
    }

    // اگر با + نداره و 0 هم نداره، فرض کن یه شماره ناقصه، ممکنه خطا بده
    if (!p.startsWith("+98")) {
        // اگر میخوای اینجا کار خاصی بکنی
    }

    return p;
}


void UserTableWidget::on_pushButton_2_clicked()
{
    QTableWidget* table = ui->tableWidget;
    auto selected = table->selectedRanges();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "خطا", "یک کاربر را انتخاب کنید.");
        return;
    }

    int row = selected.first().topRow();

    QString phone = normalizePhoneNumber(table->item(row, 1)->text());
    qDebug()<< "phone "<<phone;
    if (phone.startsWith("+"))
        phone = phone.mid(1);
    QString status = table->item(row, 3)->text().toLower();

    QString newStatus;
    if (status == "blocked" || status == "بلاک") {
        newStatus = "active";
    } else {
        newStatus = "blocked";
    }

    QString msg = QString("TOGGLE_USER_STATUS:%1:%2\n").arg(phone).arg(newStatus);
    qDebug() << "Sending:" << msg;

    if (clientSocket) {
        clientSocket->sendMessage(msg);
    } else {
        QMessageBox::warning(this, "خطا", "اتصال با سرور برقرار نیست.");
    }
}
