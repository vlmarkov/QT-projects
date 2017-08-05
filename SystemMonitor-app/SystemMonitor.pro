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
            libs/cpuid/libcpuid.h \
            cpumonitor.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -O3

CONFIG += c++11 -pthread

INCLUDEPATH += libs

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/cpuid/release/ -lcpuid
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/cpuid/debug/ -lcpuid
else:unix: LIBS += -L$$PWD/libs/cpuid/ -lcpuid

INCLUDEPATH += $$PWD/libs/cpuid
DEPENDPATH += $$PWD/libs/cpuid

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libs/cpuid/release/libcpuid.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libs/cpuid/debug/libcpuid.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libs/cpuid/release/cpuid.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libs/cpuid/debug/cpuid.lib
else:unix: PRE_TARGETDEPS += $$PWD/libs/cpuid/libcpuid.a
