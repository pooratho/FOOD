/********************************************************************************
** Form generated from reading UI file 'restaurantownersignupwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RESTAURANTOWNERSIGNUPWINDOW_H
#define UI_RESTAURANTOWNERSIGNUPWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RestaurantOwnerSignUpWindow

{
public:
    QWidget *widget;
    QWidget *widget_2;
    QLineEdit *lineEditRestaurantName;
    QLineEdit *lineEditOwnerFirstName;
    QLineEdit *lineEditPhone;
    QLabel *label_4;
    QLineEdit *lineEditPassword;
    QLineEdit *lineEditPasswordrepeat;
    QLineEdit *lineEditOwnerLastName;
    QComboBox *comboBoxProvince;
    QComboBox *comboBoxCity;
    QPushButton *pushButton;
    QWidget *widget_3;

    void setupUi(QWidget *RestaurantOwnerSignUpWindow_)
    {
        if (RestaurantOwnerSignUpWindow_->objectName().isEmpty())
            RestaurantOwnerSignUpWindow_->setObjectName("RestaurantOwnerSignUpWindow_");
        RestaurantOwnerSignUpWindow_->resize(563, 689);
        RestaurantOwnerSignUpWindow_->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
        RestaurantOwnerSignUpWindow_->setStyleSheet(QString::fromUtf8(""));
        widget = new QWidget(RestaurantOwnerSignUpWindow_);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(-70, -30, 661, 711));
        widget->setStyleSheet(QString::fromUtf8("x"));
        widget_2 = new QWidget(widget);
        widget_2->setObjectName("widget_2");
        widget_2->setGeometry(QRect(205, 50, 281, 581));
        widget_2->setStyleSheet(QString::fromUtf8("color: rgb(240, 80, 0);\n"
"font: 12pt ;\n"
"font: 12pt \"B Farnaz\";\n"
"background-color: rgb(70, 70, 70);\n"
""));
        lineEditRestaurantName = new QLineEdit(widget_2);
        lineEditRestaurantName->setObjectName("lineEditRestaurantName");
        lineEditRestaurantName->setGeometry(QRect(40, 80, 201, 41));
        lineEditRestaurantName->setStyleSheet(QString::fromUtf8(""));
        lineEditOwnerFirstName = new QLineEdit(widget_2);
        lineEditOwnerFirstName->setObjectName("lineEditOwnerFirstName");
        lineEditOwnerFirstName->setGeometry(QRect(40, 140, 201, 41));
        lineEditPhone = new QLineEdit(widget_2);
        lineEditPhone->setObjectName("lineEditPhone");
        lineEditPhone->setGeometry(QRect(40, 260, 201, 41));
        lineEditPhone->setText(QString::fromUtf8(""));
        lineEditPhone->setPlaceholderText(QString::fromUtf8("  \330\264\331\205\330\247\330\261\331\207 \331\205\331\210\330\250\330\247\333\214\331\204                      98+"));
        label_4 = new QLabel(widget_2);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(80, 10, 171, 61));
        label_4->setStyleSheet(QString::fromUtf8("font: 28pt \"B Farnaz\";"));
        lineEditPassword = new QLineEdit(widget_2);
        lineEditPassword->setObjectName("lineEditPassword");
        lineEditPassword->setGeometry(QRect(40, 320, 201, 41));
        lineEditPasswordrepeat = new QLineEdit(widget_2);
        lineEditPasswordrepeat->setObjectName("lineEditPasswordrepeat");
        lineEditPasswordrepeat->setGeometry(QRect(40, 380, 201, 41));
        lineEditOwnerLastName = new QLineEdit(widget_2);
        lineEditOwnerLastName->setObjectName("lineEditOwnerLastName");
        lineEditOwnerLastName->setGeometry(QRect(40, 200, 201, 41));
        comboBoxProvince = new QComboBox(widget_2);
        comboBoxProvince->addItem(QString());
        comboBoxProvince->addItem(QString());
        comboBoxProvince->setObjectName("comboBoxProvince");
        comboBoxProvince->setGeometry(QRect(40, 440, 201, 41));
        comboBoxCity = new QComboBox(widget_2);
        comboBoxCity->addItem(QString());
        comboBoxCity->addItem(QString());
        comboBoxCity->setObjectName("comboBoxCity");
        comboBoxCity->setGeometry(QRect(40, 500, 201, 41));
        pushButton = new QPushButton(widget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(270, 660, 151, 31));
        pushButton->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 85, 0);\n"
""));
        widget_3 = new QWidget(RestaurantOwnerSignUpWindow_);
        widget_3->setObjectName("widget_3");
        widget_3->setGeometry(QRect(-10, -10, 581, 781));
        widget_3->setStyleSheet(QString::fromUtf8("background-image: url(\"C:/Users/Taheri/OneDrive/\330\257\330\263\332\251\330\252\330\247\331\276/foodSource/hg5.jpg\");\n"
"background-repeat: no-repeat;\n"
"background-position: center;\n"
""));
        widget_3->raise();
        widget->raise();

        retranslateUi(RestaurantOwnerSignUpWindow_);

        QMetaObject::connectSlotsByName(RestaurantOwnerSignUpWindow_);
    } // setupUi
    void retranslateUi(QWidget *RestaurantOwnerSignUpWindow_)
    {
        RestaurantOwnerSignUpWindow_->setWindowTitle(QCoreApplication::translate("RestaurantOwnerSignUpWindow", "Form", nullptr));
        lineEditRestaurantName->setPlaceholderText(QCoreApplication::translate("RestaurantOwnerSignUpWindow", "  نام رستوران", nullptr));
        lineEditOwnerFirstName->setPlaceholderText(QCoreApplication::translate("RestaurantOwnerSignUpWindow", "  نام رستوراندار", nullptr));
#if QT_CONFIG(tooltip)
        lineEditPhone->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
        label_4->setText(QCoreApplication::translate("RestaurantOwnerSignUpWindow", "ثبت نام", nullptr));
        lineEditPassword->setPlaceholderText(QCoreApplication::translate("RestaurantOwnerSignUpWindow", "  رمز عبور", nullptr));
        lineEditPasswordrepeat->setPlaceholderText(QCoreApplication::translate("RestaurantOwnerSignUpWindow", "  تکرار رمز عبور", nullptr));
        lineEditOwnerLastName->setPlaceholderText(QCoreApplication::translate("RestaurantOwnerSignUpWindow", "  نام خانوادگی رستوراندار", nullptr));
        comboBoxProvince->setItemText(0, QCoreApplication::translate("RestaurantOwnerSignUpWindow", "استان"));
        comboBoxProvince->setItemText(1, QCoreApplication::translate("RestaurantOwnerSignUpWindow", "تهران"));
        comboBoxProvince->setPlaceholderText(QCoreApplication::translate("RestaurantOwnerSignUpWindow", "  انتخاب استان", nullptr));
        comboBoxCity->setItemText(0, QCoreApplication::translate("RestaurantOwnerSignUpWindow", "شهر"));
        comboBoxCity->setItemText(1, QCoreApplication::translate("RestaurantOwnerSignUpWindow", "تهران"));
        comboBoxCity->setPlaceholderText(QCoreApplication::translate("RestaurantOwnerSignUpWindow", "  انتخاب شهر", nullptr));
        pushButton->setText(QCoreApplication::translate("RestaurantOwnerSignUpWindow", "تایید", nullptr));
    }

};

namespace Ui {
    class RestaurantOwnerSignUpWindow
: public Ui_RestaurantOwnerSignUpWindow
 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESTAURANTOWNERSIGNUPWINDOW_H
