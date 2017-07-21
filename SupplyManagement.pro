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
    HumanPage.cpp \
    AboutPage.cpp \
    SalesPage.cpp \
    OrderPage.cpp \
    ProdsPage.cpp \
    PurchPage.cpp \
    LoginPage.cpp \
    UdpSocket.cpp

HEADERS  += mainscreen.h \
    delegate.h \
    HumanPage.h \
    AboutPage.h \
    SalesPage.h \
    OrderPage.h \
    ProdsPage.h \
    PurchPage.h \
    LoginPage.h \
    UdpSocket.h

RESOURCES += \
    source.qrc

RC_FILE += ico.rc
