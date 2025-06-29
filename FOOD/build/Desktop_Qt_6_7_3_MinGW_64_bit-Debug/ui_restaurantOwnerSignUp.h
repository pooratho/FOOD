/********************************************************************************
** Form generated from reading UI file 'restaurantownersignup.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RESTAURANTOWNERSIGNUP_H
#define UI_RESTAURANTOWNERSIGNUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RestaurantOwnerSignUp
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

    void setupUi(QWidget *RestaurantOwnerSignUp)
    {
        if (RestaurantOwnerSignUp->objectName().isEmpty())
            RestaurantOwnerSignUp->setObjectName("RestaurantOwnerSignUp");
        RestaurantOwnerSignUp->resize(563, 689);
        RestaurantOwnerSignUp->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
        RestaurantOwnerSignUp->setStyleSheet(QString::fromUtf8(""));
        widget = new QWidget(RestaurantOwnerSignUp);
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
        widget_3 = new QWidget(RestaurantOwnerSignUp);
        widget_3->setObjectName("widget_3");
        widget_3->setGeometry(QRect(-10, -10, 581, 781));
        widget_3->setStyleSheet(QString::fromUtf8("background-image: url(:/background.jpg);"));
        widget_3->raise();
        widget->raise();

        retranslateUi(RestaurantOwnerSignUp);

        QMetaObject::connectSlotsByName(RestaurantOwnerSignUp);
    } // setupUi

    void retranslateUi(QWidget *RestaurantOwnerSignUp)
    {
        RestaurantOwnerSignUp->setWindowTitle(QCoreApplication::translate("RestaurantOwnerSignUp", "Form", nullptr));
        lineEditRestaurantName->setPlaceholderText(QCoreApplication::translate("RestaurantOwnerSignUp", "  \331\206\330\247\331\205 \330\261\330\263\330\252\331\210\330\261\330\247\331\206", nullptr));
        lineEditOwnerFirstName->setPlaceholderText(QCoreApplication::translate("RestaurantOwnerSignUp", "  \331\206\330\247\331\205 \331\205\330\247\331\204\332\251 \331\201\330\261\331\210\330\264\332\257\330\247\331\207", nullptr));
#if QT_CONFIG(tooltip)
        lineEditPhone->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
        label_4->setText(QCoreApplication::translate("RestaurantOwnerSignUp", "  \330\271\330\266\331\210\333\214\330\252", nullptr));
        lineEditPassword->setPlaceholderText(QCoreApplication::translate("RestaurantOwnerSignUp", "  \330\261\331\205\330\262 \330\271\330\250\331\210\330\261", nullptr));
        lineEditPasswordrepeat->setPlaceholderText(QCoreApplication::translate("RestaurantOwnerSignUp", "  \330\252\332\251\330\261\330\247\330\261 \330\261\331\205\330\262 \330\271\330\250\331\210\330\261", nullptr));
        lineEditOwnerLastName->setPlaceholderText(QCoreApplication::translate("RestaurantOwnerSignUp", "  \331\206\330\247\331\205 \330\256\330\247\331\206\331\210\330\247\330\257\332\257\333\214 \331\205\330\247\331\204\332\251 \331\201\330\261\331\210\330\264\332\257\330\247\331\207", nullptr));
        comboBoxProvince->setItemText(0, QCoreApplication::translate("RestaurantOwnerSignUp", "\330\252\331\207\330\261\330\247\331\206", nullptr));
        comboBoxProvince->setItemText(1, QCoreApplication::translate("RestaurantOwnerSignUp", "\330\247\330\265\331\201\331\207\330\247\331\206", nullptr));

        comboBoxProvince->setPlaceholderText(QCoreApplication::translate("RestaurantOwnerSignUp", "  \330\247\331\206\330\252\330\256\330\247\330\250 \330\247\330\263\330\252\330\247\331\206", nullptr));
        comboBoxCity->setItemText(0, QCoreApplication::translate("RestaurantOwnerSignUp", "\330\252\331\207\330\261\330\247\331\206", nullptr));
        comboBoxCity->setItemText(1, QCoreApplication::translate("RestaurantOwnerSignUp", "\330\247\330\265\331\201\331\207\330\247\331\206", nullptr));

        comboBoxCity->setPlaceholderText(QCoreApplication::translate("RestaurantOwnerSignUp", "  \330\247\331\206\330\252\330\256\330\247\330\250 \330\264\331\207\330\261", nullptr));
        pushButton->setText(QCoreApplication::translate("RestaurantOwnerSignUp", "\330\252\330\247\333\214\333\214\330\257", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RestaurantOwnerSignUp: public Ui_RestaurantOwnerSignUp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESTAURANTOWNERSIGNUP_H
