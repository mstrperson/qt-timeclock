#-------------------------------------------------
#
# Project created by QtCreator 2015-12-18T12:52:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Signin
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

QT += sql

QMAKE_CXXFLAGS += -std=c++0x
