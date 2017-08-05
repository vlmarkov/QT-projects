#-------------------------------------------------
#
# Project created by QtCreator 2017-07-29T20:02:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = SystemMonitor
TEMPLATE = app


SOURCES += main.cpp\
           mainwindow.cpp \
           cpumonitor.cpp \
           libs/qcustomplot/qcustomplot.cpp

HEADERS  += mainwindow.h \
            abstractsystemmonitor.h \
            libs/qcustomplot/qcustomplot.h \
            libs/cpuid/libcpuid_constants.h \
            libs/cpuid/libcpuid_types.h \
            libs/cpuid/libcpuid.h

FORMS    += mainwindow.ui

CONFIG += c++11 -pthread

LIBS += -L/libs/cpuid -lcpuid
INCLUDEPATH += libs
