#-------------------------------------------------
#
# Project created by QtCreator 2017-05-16T19:38:05
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++17

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CrcCalculator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    crcutils.h

FORMS    += mainwindow.ui
