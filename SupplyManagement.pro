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
    aboutusscreen.cpp \
    udpsocket.cpp \
    usermanagerment.cpp \
    rolemanagerment.cpp \
    ordermanagement.cpp \
    productionmanagement.cpp \
    lackmanagement.cpp \
    purchasemanagement.cpp

HEADERS  += mainscreen.h \
    loginscreen.h \
    aboutusscreen.h \
    udpsocket.h \
    usermanagerment.h \
    delegate.h \
    rolemanagerment.h \
    ordermanagement.h \
    productionmanagement.h \
    lackmanagement.h \
    purchasemanagement.h

RESOURCES += \
    source.qrc
