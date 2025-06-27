QT       += core gui
QT += sql
QT += core gui sql
QT += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    customersignup.cpp \
    database.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    restaurantownersignupwindow.cpp \
    signinwindow.cpp

HEADERS += \
    customersignup.h \
    database.h \
    loginwindow.h \
    mainwindow.h \
    restaurantownersignupwindow.h \
    signinwindow.h

FORMS += \
    customersignup.ui \
    mainwindow.ui \
    login.ui \
    restaurantOwnerSignUp.ui \
    signIn.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
