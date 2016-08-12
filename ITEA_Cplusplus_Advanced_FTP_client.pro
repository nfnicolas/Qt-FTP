QT += core network
QT -= gui

TARGET = ITEA_Cplusplus_Advanced_FTP_client
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    client.cpp

HEADERS += \
    client.h

