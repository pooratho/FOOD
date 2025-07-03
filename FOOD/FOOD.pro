QT += core gui network sql
QT += quickwidgets

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
    orderitemwidget.cpp \
    orderstablewidget.cpp \
    restaurantmenu.cpp \
    restaurantmenuitemwidget.cpp \
    restaurantownermainpage.cpp \
    restaurantownermenuitemwidget.cpp \
    restaurantownerorderitemwidget.cpp \
    restaurantownersignup.cpp \
    menuitem.cpp \
    order.cpp \
    restaurant.cpp \
    restaurantowner.cpp \
    restauranttablewidget.cpp \
    shoppingcartitemwidget.cpp \
    signinwindow.cpp \
    user.cpp \
    userstablewidget.cpp

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
    orderitemwidget.h \
    orderstablewidget.h \
    restaurantmenuitemwidget.h \
    restaurantownermainpage.h \
    restaurantownermenuitemwidget.h \
    restaurantownersignup.h \
    restauranttablewidget.h \
    shoppingcartitemwidget.h \
    signinwindow.h \
    menuitem.h \
    order.h \
    restaurant.h \
    restaurantowner.h \
    user.h \
    restaurantmenu.h \
    restaurantownerorderitemwidget.h \
    userstablewidget.h

FORMS += \
    adminmainpage.ui \
    customermainpage.ui \
    customersignup.ui \
    loginwindow.ui \
    mainwindow.ui \
    restaurantownermainpage.ui \
    restaurantownersignup.ui \
    signinwindow.ui \
    restaurantmenuitemwidget.ui \
    restaurantownermenuitemwidget.ui \
    restaurantmenu.ui \
    shoppingcartitemwidget.ui \
    restaurantownerorderitemwidget.ui \
    orderitemwidget.ui \
    restauranttablewidget.ui \
    userstablewidget.ui \
    orderstablewidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
