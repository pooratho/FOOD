/********************************************************************************
** Form generated from reading UI file 'signinwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIGNINWINDOW_H
#define UI_SIGNINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SignInWindow
{
public:
    QWidget *widget;
    QWidget *widget_2;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *lineEditFirstName;
    QLineEdit *lineEditLastName;
    QLabel *label_4;
    QLineEdit *lineEditPassword;
    QWidget *widget_3;
    QPushButton *pushButton;

    void setupUi(QWidget *SignInWindow)
    {
        if (SignInWindow->objectName().isEmpty())
            SignInWindow->setObjectName("SignInWindow");
        SignInWindow->resize(509, 552);
        SignInWindow->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
        SignInWindow->setStyleSheet(QString::fromUtf8(""));
        widget = new QWidget(SignInWindow);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(-70, -30, 661, 711));
        widget->setStyleSheet(QString::fromUtf8("x"));
        widget_2 = new QWidget(widget);
        widget_2->setObjectName("widget_2");
        widget_2->setGeometry(QRect(180, 60, 281, 441));
        widget_2->setStyleSheet(QString::fromUtf8("color: rgb(240, 80, 0);\n"
"background-color: rgb(70, 70, 70);\n"
"font: 12pt ;\n"
"font: 12pt \"B Farnaz\";"));
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
        label_4 = new QLabel(widget_2);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(80, 15, 171, 61));
        label_4->setStyleSheet(QString::fromUtf8("font: 28pt \"B Farnaz\";"));
        lineEditPassword = new QLineEdit(widget_2);
        lineEditPassword->setObjectName("lineEditPassword");
        lineEditPassword->setGeometry(QRect(40, 355, 201, 41));
        widget_3 = new QWidget(widget);
        widget_3->setObjectName("widget_3");
        widget_3->setGeometry(QRect(0, 0, 611, 621));
        widget_3->setStyleSheet(QString::fromUtf8("background-image: url(\"C:/Users/Taheri/OneDrive/\330\257\330\263\332\251\330\252\330\247\331\276/foodSource/hg4.jpg\");\n"
"background-repeat: no-repeat;\n"
"background-position: center;\n"
""));
        pushButton = new QPushButton(widget_3);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(250, 520, 131, 31));
        pushButton->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 85, 0);"));
        widget_3->raise();
        widget_2->raise();

        retranslateUi(SignInWindow);

        QMetaObject::connectSlotsByName(SignInWindow);
    } // setupUi

    void retranslateUi(QWidget *SignInWindow)
    {
        SignInWindow->setWindowTitle(QCoreApplication::translate("SignInWindow", "Form", nullptr));
        label->setText(QCoreApplication::translate("SignInWindow", "  \331\206\330\247\331\205:", nullptr));
        label_2->setText(QCoreApplication::translate("SignInWindow", "  \331\206\330\247\331\205 \330\256\331\210\330\247\331\206\331\210\330\247\330\257\332\257\333\214:", nullptr));
        label_3->setText(QCoreApplication::translate("SignInWindow", "  \330\261\331\205\330\262 \330\271\330\250\331\210\330\261:", nullptr));
        label_4->setText(QCoreApplication::translate("SignInWindow", "  \331\210\330\261\331\210\330\257", nullptr));
        pushButton->setText(QCoreApplication::translate("SignInWindow", "\330\252\330\247\333\214\333\214\330\257", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SignInWindow: public Ui_SignInWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIGNINWINDOW_H
