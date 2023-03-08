#-------------------------------------------------
#
# Project created by QtCreator 2018-03-22T08:55:09
#
#-------------------------------------------------

QT       += core gui

lessThan(QT_MAJOR_VERSION, 5): QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets
  DEFINES += HAVE_QT5
}

TARGET = UserInterfaceReciver
TEMPLATE = app

INCLUDEPATH += $$PWD/../../UtilityUWB
INCLUDEPATH += $$PWD/../Commands
SOURCES += main.cpp \
    rs485dialogbox.cpp \
    ptreecommandswriter.cpp \
    ../../UtilityUWB/ptreewr.cpp \
    ../../UtilityUWB/networkuwb.cpp \
    ../../UtilityUWB/errorcodeboost.cpp

HEADERS  += \
    rs485dialogbox.h \
    ptreecommandswriter.h \    
    ../../UtilityUWB/ptreewr.h \
    ../../UtilityUWB/networkuwb.h \
    ../../UtilityUWB/errorcodeboost.h

FORMS += \
    rs485dialogbox.ui

LIBS += -lboost_system
LIBS += -pthread
