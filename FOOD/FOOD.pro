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
    cartitem.cpp \
    clientsocketmanager.cpp \
    customer.cpp \
    customersignup.cpp \
    food.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    menuitem.cpp \
    order.cpp \
    restaurant.cpp \
    restaurantowner.cpp \
    restaurantownersignupwindow.cpp \
    signinwindow.cpp \
    user.cpp

HEADERS += \
    cartitem.h \
    clientsocketmanager.h \
    customer.h \
    customersignup.h \
    food.h \
    loginwindow.h \
    mainwindow.h \
    menuitem.h \
    order.h \
    restaurant.h \
    restaurantowner.h \
    restaurantownersignupwindow.h \
    signinwindow.h \
    user.h

FORMS += \
    customersignup.ui \
    loginwindow.ui \
    mainwindow.ui \
    restaurantownersignupwindow.ui \
    signinwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
