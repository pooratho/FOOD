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
    adminmainpage.cpp \
    cartitem.cpp \
    clientsocketmanager.cpp \
    customermainpage.cpp \
    customer.cpp \
    customersignup.cpp \
    food.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    restaurantownermainpage.cpp \
    restaurantownersignup.cpp \
    menuitem.cpp \
    order.cpp \
    restaurant.cpp \
    restaurantowner.cpp \
    signinwindow.cpp \
    user.cpp

HEADERS += \
    adminmainpage.h \
    cartitem.h \
    clientsocketmanager.h \
    customermainpage.h \
    customer.h \
    customersignup.h \
    food.h \
    loginwindow.h \
    mainwindow.h \
    restaurantownermainpage.h \
    restaurantownersignup.h \
    signinwindow.h \
    menuitem.h \
    order.h \
    restaurant.h \
    restaurantowner.h \
    user.h

FORMS += \
    adminmainpage.ui \
    customersignup.ui \
    loginwindow.ui \
    mainwindow.ui \
    restaurantownersignup.ui \
    signinwindow.ui \
    customerMainPage.ui \
    restaurantOwnerMainPage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
