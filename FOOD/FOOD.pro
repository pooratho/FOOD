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
    clientsocketmanager.cpp \
    customermainpage.cpp \
    customersignup.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    restaurantownersignup.cpp \
    signinwindow.cpp

HEADERS += \
    clientsocketmanager.h \
    customermainpage.h \
    customersignup.h \
    loginwindow.h \
    mainwindow.h \
    restaurantownersignup.h \
    signinwindow.h

FORMS += \
    customersignup.ui \
    loginwindow.ui \
    mainwindow.ui \
    restaurantownersignup.ui \
    signinwindow.ui \
    customerMainPage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
