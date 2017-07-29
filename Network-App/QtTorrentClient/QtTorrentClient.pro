#-------------------------------------------------
#
# Project created by QtCreator 2017-07-15T23:18:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtTorrentClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    abstractlogger.h \
    abstractfilemanager.h

FORMS    += mainwindow.ui
