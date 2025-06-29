/********************************************************************************
** Form generated from reading UI file 'customerSignUp.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUSTOMERSIGNUP_H
#define UI_CUSTOMERSIGNUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QWidget *widget;
    QWidget *widget_2;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_5;
    QPushButton *pushButton;
    QWidget *widget_3;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName("Form");
        Form->resize(563, 754);
        Form->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
        Form->setStyleSheet(QString::fromUtf8(""));
        widget = new QWidget(Form);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(-70, -30, 661, 711));
        widget->setStyleSheet(QString::fromUtf8("x"));
        widget_2 = new QWidget(widget);
        widget_2->setObjectName("widget_2");
        widget_2->setGeometry(QRect(205, 50, 281, 651));
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
        lineEdit = new QLineEdit(widget_2);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(40, 125, 201, 41));
        lineEdit->setStyleSheet(QString::fromUtf8(""));
        lineEdit_2 = new QLineEdit(widget_2);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(40, 235, 201, 41));
        lineEdit_3 = new QLineEdit(widget_2);
        lineEdit_3->setObjectName("lineEdit_3");
        lineEdit_3->setGeometry(QRect(40, 355, 201, 41));
        lineEdit_3->setText(QString::fromUtf8("+98"));
        lineEdit_3->setPlaceholderText(QString::fromUtf8("+98"));
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
        lineEdit_4 = new QLineEdit(widget_2);
        lineEdit_4->setObjectName("lineEdit_4");
        lineEdit_4->setGeometry(QRect(40, 465, 201, 41));
        lineEdit_5 = new QLineEdit(widget_2);
        lineEdit_5->setObjectName("lineEdit_5");
        lineEdit_5->setGeometry(QRect(40, 570, 201, 41));
        pushButton = new QPushButton(Form);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(200, 690, 151, 31));
        pushButton->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 85, 0);\n"
""));
        widget_3 = new QWidget(Form);
        widget_3->setObjectName("widget_3");
        widget_3->setGeometry(QRect(-10, -10, 581, 781));
        widget_3->setStyleSheet(QString::fromUtf8("background-image: url(\"C:/Users/Taheri/OneDrive/\330\257\330\263\332\251\330\252\330\247\331\276/foodSource/hg5.jpg\");\n"
"background-repeat: no-repeat;\n"
"background-position: center;\n"
""));
        widget_3->raise();
        widget->raise();
        pushButton->raise();

        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QCoreApplication::translate("Form", "Form", nullptr));
        label->setText(QCoreApplication::translate("Form", "  \331\206\330\247\331\205:", nullptr));
        label_2->setText(QCoreApplication::translate("Form", "  \331\206\330\247\331\205 \330\256\331\210\330\247\331\206\331\210\330\247\330\257\332\257\333\214:", nullptr));
        label_3->setText(QCoreApplication::translate("Form", "  \330\264\331\205\330\247\330\261\331\207 \331\205\331\210\330\250\330\247\333\214\331\204:", nullptr));
#if QT_CONFIG(tooltip)
        lineEdit_3->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
        label_4->setText(QCoreApplication::translate("Form", "  \330\271\330\266\331\210\333\214\330\252", nullptr));
        label_5->setText(QCoreApplication::translate("Form", "  \330\261\331\205\330\262 \330\271\330\250\331\210\330\261:", nullptr));
        label_6->setText(QCoreApplication::translate("Form", "  \330\252\332\251\330\261\330\247\330\261 \330\261\331\205\330\262 \330\271\330\250\331\210\330\261:", nullptr));
        pushButton->setText(QCoreApplication::translate("Form", "\330\252\330\247\333\214\333\214\330\257", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUSTOMERSIGNUP_H
