/********************************************************************************
** Form generated from reading UI file 'customersignup.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUSTOMERSIGNUP_H
#define UI_CUSTOMERSIGNUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_customersignup
{
public:
    QWidget *widget;
    QWidget *widget_2;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *lineEditFirstName;
    QLineEdit *lineEditLastName;
    QLineEdit *lineEditPhone;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLineEdit *lineEditPassword;
    QLineEdit *lineEditPasswordRepeat;
    QPushButton *pushButton;

    void setupUi(QDialog *customersignup)
    {
        if (customersignup->objectName().isEmpty())
            customersignup->setObjectName("customersignup");
        customersignup->resize(596, 814);
        widget = new QWidget(customersignup);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(10, 0, 561, 711));
        widget->setStyleSheet(QString::fromUtf8("x"));
        widget_2 = new QWidget(widget);
        widget_2->setObjectName("widget_2");
        widget_2->setGeometry(QRect(130, 60, 281, 651));
        widget_2->setStyleSheet(QString::fromUtf8("color: rgb(240, 80, 0);\n"
"font: 12pt ;\n"
"font: 12pt \"B Farnaz\";\n"
"background-color: rgb(70, 70, 70);\n"
""));
        label = new QLabel(widget_2);
        label->setObjectName("label");
        label->setGeometry(QRect(100, 85, 141, 20));
        label->setStyleSheet(QString::fromUtf8("color: rgb(240, 80, 0);\n"
"font: 12pt ;\n"
"font: 12pt \"B Farnaz\";"));
        label_2 = new QLabel(widget_2);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(50, 195, 191, 20));
        label_3 = new QLabel(widget_2);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(50, 310, 191, 20));
        lineEditFirstName = new QLineEdit(widget_2);
        lineEditFirstName->setObjectName("lineEditFirstName");
        lineEditFirstName->setGeometry(QRect(40, 125, 201, 41));
        lineEditFirstName->setStyleSheet(QString::fromUtf8(""));
        lineEditLastName = new QLineEdit(widget_2);
        lineEditLastName->setObjectName("lineEditLastName");
        lineEditLastName->setGeometry(QRect(40, 235, 201, 41));
        lineEditPhone = new QLineEdit(widget_2);
        lineEditPhone->setObjectName("lineEditPhone");
        lineEditPhone->setGeometry(QRect(40, 355, 201, 41));
        lineEditPhone->setText(QString::fromUtf8("+98"));
        lineEditPhone->setPlaceholderText(QString::fromUtf8("+98"));
        label_4 = new QLabel(widget_2);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(80, 15, 171, 61));
        label_4->setStyleSheet(QString::fromUtf8("font: 28pt \"B Farnaz\";"));
        label_5 = new QLabel(widget_2);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(140, 425, 101, 20));
        label_6 = new QLabel(widget_2);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(140, 530, 101, 20));
        lineEditPassword = new QLineEdit(widget_2);
        lineEditPassword->setObjectName("lineEditPassword");
        lineEditPassword->setGeometry(QRect(40, 465, 201, 41));
        lineEditPasswordRepeat = new QLineEdit(widget_2);
        lineEditPasswordRepeat->setObjectName("lineEditPasswordRepeat");
        lineEditPasswordRepeat->setGeometry(QRect(40, 570, 201, 41));
        pushButton = new QPushButton(customersignup);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(200, 750, 151, 31));
        pushButton->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 85, 0);\n"
""));

        retranslateUi(customersignup);

        QMetaObject::connectSlotsByName(customersignup);
    } // setupUi

    void retranslateUi(QDialog *customersignup)
    {
        customersignup->setWindowTitle(QCoreApplication::translate("customersignup", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("customersignup", "  \331\206\330\247\331\205:", nullptr));
        label_2->setText(QCoreApplication::translate("customersignup", "  \331\206\330\247\331\205 \330\256\331\210\330\247\331\206\331\210\330\247\330\257\332\257\333\214:", nullptr));
        label_3->setText(QCoreApplication::translate("customersignup", "  \330\264\331\205\330\247\330\261\331\207 \331\205\331\210\330\250\330\247\333\214\331\204:", nullptr));
#if QT_CONFIG(tooltip)
        lineEditPhone->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
        label_4->setText(QCoreApplication::translate("customersignup", "  \330\271\330\266\331\210\333\214\330\252", nullptr));
        label_5->setText(QCoreApplication::translate("customersignup", "  \330\261\331\205\330\262 \330\271\330\250\331\210\330\261:", nullptr));
        label_6->setText(QCoreApplication::translate("customersignup", "  \330\252\332\251\330\261\330\247\330\261 \330\261\331\205\330\262 \330\271\330\250\331\210\330\261:", nullptr));
        pushButton->setText(QCoreApplication::translate("customersignup", "\330\252\330\247\333\214\333\214\330\257", nullptr));
    } // retranslateUi

};

namespace Ui {
    class customersignup: public Ui_customersignup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUSTOMERSIGNUP_H
