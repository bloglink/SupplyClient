#-------------------------------------------------
#
# Project created by QtCreator 2017-07-07T15:25:17
#
#-------------------------------------------------

QT       += core gui network xml sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SupplyManagement
TEMPLATE = app


SOURCES += main.cpp\
        mainscreen.cpp \
    loginscreen.cpp \
    udpsocket.cpp \
    productionmanagement.cpp \
    lackmanagement.cpp \
    purchasemanagement.cpp \
    erpsql.cpp \
    HumanPage.cpp \
    AboutPage.cpp \
    SalesPage.cpp \
    OrderPage.cpp \
    ProdPage.cpp

HEADERS  += mainscreen.h \
    loginscreen.h \
    udpsocket.h \
    delegate.h \
    productionmanagement.h \
    lackmanagement.h \
    purchasemanagement.h \
    erpsql.h \
    HumanPage.h \
    AboutPage.h \
    SalesPage.h \
    OrderPage.h \
    ProdPage.h

RESOURCES += \
    source.qrc
