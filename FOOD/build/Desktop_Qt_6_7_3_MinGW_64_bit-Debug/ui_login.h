/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QLabel *label;
    QWidget *widget;
    QComboBox *comboBox;
    QLabel *label_2;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QWidget *widget_2;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName("Form");
        Form->resize(552, 429);
        Form->setStyleSheet(QString::fromUtf8(""));
        label = new QLabel(Form);
        label->setObjectName("label");
        label->setGeometry(QRect(80, 25, 391, 81));
        label->setStyleSheet(QString::fromUtf8("font: 48pt \"AlphaFridgeMagnets \";\n"
"\n"
"color: rgb(255, 85, 0);"));
        widget = new QWidget(Form);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(89, 129, 361, 261));
        widget->setStyleSheet(QString::fromUtf8("background-color: rgb(70, 70, 70);"));
        comboBox = new QComboBox(widget);
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::CameraWeb));
        comboBox->addItem(icon, QString());
        comboBox->addItem(icon, QString());
        comboBox->addItem(icon, QString());
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(80, 60, 191, 41));
        comboBox->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
        comboBox->setAutoFillBackground(false);
        comboBox->setStyleSheet(QString::fromUtf8("color: rgb(255, 85, 0);"));
        label_2 = new QLabel(widget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(80, 10, 191, 41));
        label_2->setStyleSheet(QString::fromUtf8("font: 12pt \"Segoe UI\";\n"
"color: rgb(255, 85, 0);"));
        pushButton = new QPushButton(widget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(110, 130, 141, 41));
        pushButton->setMouseTracking(false);
        pushButton->setTabletTracking(false);
        pushButton->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 124, 10);"));
        pushButton_2 = new QPushButton(widget);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(110, 180, 141, 41));
        pushButton_2->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 124, 10);"));
        widget_2 = new QWidget(Form);
        widget_2->setObjectName("widget_2");
        widget_2->setGeometry(QRect(-40, -80, 600, 511));
        widget_2->setStyleSheet(QString::fromUtf8("background-image: url(\"C:/Users/Taheri/OneDrive/\330\257\330\263\332\251\330\252\330\247\331\276/foodSource/hg3 copy.jpg\");\n"
"background-repeat: no-repeat;\n"
"background-position: center;\n"
""));
        widget_2->raise();
        label->raise();
        widget->raise();

        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QCoreApplication::translate("Form", "Form", nullptr));
        label->setText(QCoreApplication::translate("Form", "HungerGames", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("Form", "\331\205\330\264\330\252\330\261\333\214", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("Form", "\330\261\330\263\330\252\331\210\330\261\330\247\331\206 \330\257\330\247\330\261", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("Form", "\331\205\330\257\333\214\330\261 \330\263\333\214\330\263\330\252\331\205", nullptr));

        label_2->setText(QCoreApplication::translate("Form", "\331\206\331\202\330\264 \330\256\331\210\330\257 \330\261\330\247 \330\247\331\206\330\252\330\256\330\247\330\250 \332\251\331\206\333\214\330\257", nullptr));
        pushButton->setText(QCoreApplication::translate("Form", "\331\210\330\261\331\210\330\257", nullptr));
        pushButton_2->setText(QCoreApplication::translate("Form", "\330\271\330\266\331\210\333\214\330\252", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
